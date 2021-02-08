#include "HartEng/Renderer/Mesh.h"

#include "HartEng/Core/Log.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Renderer/Material.h"
#include "HartEng/Renderer/Renderer.h"

#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <assimp/Importer.hpp>
#include <assimp/DefaultLogger.hpp>
#include <assimp/LogStream.hpp>

#include <filesystem>

namespace HE
{
	static const uint32_t s_MeshImportFlags =
		aiProcess_CalcTangentSpace |        // Create binormals/tangents just in case
		aiProcess_Triangulate |             // Make sure we're triangles
		aiProcess_SortByPType |             // Split meshes by primitive type
		aiProcess_GenNormals |              // Make sure we have legit normals
		aiProcess_GenUVCoords |             // Convert UVs if required 
		aiProcess_OptimizeMeshes |          // Batch draws where possible
		aiProcess_ValidateDataStructure;    // Validation

	glm::mat4 Mat4FromAssimpMat4(const aiMatrix4x4& matrix)
	{
		glm::mat4 result;
		//the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
		result[0][0] = matrix.a1; result[1][0] = matrix.a2; result[2][0] = matrix.a3; result[3][0] = matrix.a4;
		result[0][1] = matrix.b1; result[1][1] = matrix.b2; result[2][1] = matrix.b3; result[3][1] = matrix.b4;
		result[0][2] = matrix.c1; result[1][2] = matrix.c2; result[2][2] = matrix.c3; result[3][2] = matrix.c4;
		result[0][3] = matrix.d1; result[1][3] = matrix.d2; result[2][3] = matrix.d3; result[3][3] = matrix.d4;
		return result;
	}

	struct LogStream : public Assimp::LogStream
	{
		static void Initialize()
		{
			if (Assimp::DefaultLogger::isNullLogger())
			{
				Assimp::DefaultLogger::create("", Assimp::Logger::VERBOSE);
				Assimp::DefaultLogger::get()->attachStream(new LogStream, Assimp::Logger::Err | Assimp::Logger::Warn);
			}
		}

		virtual void write(const char* message) override
		{
			HE_CORE_ERROR("Assimp: {0}", message);
		}
	};

	Mesh::Mesh(const std::string& filename) :
		m_FilePath(filename)
	{
		LogStream::Initialize();

		HE_CORE_INFO("Loading mesh: {0}", filename.c_str());

		m_Importer = std::make_unique<Assimp::Importer>();

		const aiScene* scene = m_Importer->ReadFile(filename, s_MeshImportFlags);
		if (!scene || !scene->HasMeshes())
		{
			HE_CORE_ERROR("Failed to load mesh file: {0}", filename);
			//return;
		}

		m_Scene = scene;

		m_MeshShader = Renderer::GetShaderLibrary()->Get("Black");
		m_BaseMaterial = std::shared_ptr<Material>(new Material(m_MeshShader));
		m_InverseTransform = glm::inverse(Mat4FromAssimpMat4(scene->mRootNode->mTransformation));

		uint32_t vertexCount = 0;
		uint32_t indexCount = 0;

		m_Submeshes.reserve(scene->mNumMeshes);
		for (size_t m = 0; m < scene->mNumMeshes; m++)
		{
			aiMesh* mesh = scene->mMeshes[m];

			Submesh& submesh = m_Submeshes.emplace_back();
			submesh.BaseVertex = vertexCount;
			submesh.BaseIndex = indexCount;
			submesh.MaterialIndex = mesh->mMaterialIndex;
			submesh.IndexCount = mesh->mNumFaces * 3;
			submesh.VertexCount = mesh->mNumVertices;
			submesh.MeshName = mesh->mName.C_Str();

			vertexCount += mesh->mNumVertices;
			indexCount += submesh.IndexCount;

			HE_CORE_ASSERT(mesh->HasPositions(), "Meshes require positions.");
			HE_CORE_ASSERT(mesh->HasNormals(), "Meshes require normals.");

			


			for (size_t i = 0; i < mesh->mNumVertices; i++)
			{
				Vertex vertex;
				vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
				vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

				if (mesh->HasTangentsAndBitangents())
				{
					vertex.Tangent = { mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z };
					vertex.Binormal = { mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z };
				}

				if (mesh->HasTextureCoords(0))
					vertex.Texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };

				m_StaticVertices.push_back(vertex);
			}

			// Indices
			for (size_t i = 0; i < mesh->mNumFaces; i++)
			{
				HE_CORE_ASSERT(mesh->mFaces[i].mNumIndices == 3, "Must have 3 indices.");
				Index index = { mesh->mFaces[i].mIndices[0], mesh->mFaces[i].mIndices[1], mesh->mFaces[i].mIndices[2] };
				m_Indices.push_back(index);

				m_TriangleCache[m].emplace_back(m_StaticVertices[index.V1 + submesh.BaseVertex], m_StaticVertices[index.V2 + submesh.BaseVertex], m_StaticVertices[index.V3 + submesh.BaseVertex]);
			}


		}

		TraverseNodes(scene->mRootNode);


		// Materials
		if (scene->HasMaterials())
		{
			HE_CORE_INFO("---- Materials - {0} ----", filename);

			m_Textures.resize(scene->mNumMaterials);
			m_Materials.resize(scene->mNumMaterials);
			for (uint32_t i = 0; i < scene->mNumMaterials; i++)
			{
				auto aiMaterial = scene->mMaterials[i];
				auto aiMaterialName = aiMaterial->GetName();

				auto mi = std::shared_ptr<MaterialInstance>(new MaterialInstance(m_BaseMaterial, aiMaterialName.data));

				// TODO redo two sided
				mi->SetFlag(MaterialFlag::TwoSided, false);

				m_Materials[i] = mi;

				HE_CORE_INFO("  {0} (Index = {1})", aiMaterialName.data, i);
				aiString aiTexPath;
				uint32_t textureCount = aiMaterial->GetTextureCount(aiTextureType_DIFFUSE);
				HE_CORE_INFO("    TextureCount = {0}", textureCount);

				aiColor3D aiColor;
				aiMaterial->Get(AI_MATKEY_COLOR_DIFFUSE, aiColor);

				float shininess, metalness;
				if (aiMaterial->Get(AI_MATKEY_SHININESS, shininess) != aiReturn_SUCCESS)
					shininess = 80.0f; // Default value

				if (aiMaterial->Get(AI_MATKEY_REFLECTIVITY, metalness) != aiReturn_SUCCESS)
					metalness = 0.0f;

				float roughness = 1.0f - glm::sqrt(shininess / 100.0f);
				HE_CORE_INFO("    COLOR = {0}, {1}, {2}", aiColor.r, aiColor.g, aiColor.b);
				HE_CORE_INFO("    ROUGHNESS = {0}", roughness);
				bool hasAlbedoMap = aiMaterial->GetTexture(aiTextureType_DIFFUSE, 0, &aiTexPath) == AI_SUCCESS;
				if (hasAlbedoMap)
				{
					// TODO: Temp - this should be handled by filesystem
					std::filesystem::path path = filename;
					auto parentPath = path.parent_path();
					parentPath /= std::string(aiTexPath.data);
					std::string texturePath = parentPath.string();
					HE_CORE_INFO("    Albedo map path = {0}", texturePath);
					//auto texture = Texture2D::Create(texturePath, true); // true - srgb
					auto texture = Texture2D::Create(texturePath); 
					if (texture->Loaded())
					{
						m_Textures[i] = texture;
						mi->Set("u_AlbedoTexture", m_Textures[i]);
						mi->Set("u_AlbedoTexToggle", 1.0f);
					}
					else
					{
						HE_CORE_ERROR("Could not load texture: {0}", texturePath);
						// Fallback to albedo color
						mi->Set("u_AlbedoColor", glm::vec3{ aiColor.r, aiColor.g, aiColor.b });
					}
				}
				else
				{
					mi->Set("u_AlbedoColor", glm::vec3{ aiColor.r, aiColor.g, aiColor.b });
					HE_CORE_INFO("    No albedo map");
				}

				// Normal maps
				mi->Set("u_NormalTexToggle", 0.0f);
				if (aiMaterial->GetTexture(aiTextureType_NORMALS, 0, &aiTexPath) == AI_SUCCESS)
				{
					// TODO: Temp - this should be handled by filesystem
					std::filesystem::path path = filename;
					auto parentPath = path.parent_path();
					parentPath /= std::string(aiTexPath.data);
					std::string texturePath = parentPath.string();
					HE_CORE_INFO("    Normal map path = {0}", texturePath);
					auto texture = Texture2D::Create(texturePath);
					if (texture->Loaded())
					{
						mi->Set("u_NormalTexture", texture);
						mi->Set("u_NormalTexToggle", 1.0f);
					}
					else
					{
						HE_CORE_ERROR("    Could not load texture: {0}", texturePath);
					}
				}
				else
				{
					HE_CORE_INFO("    No normal map");
				}

				// Roughness map
				// mi->Set("u_Roughness", 1.0f);
				// mi->Set("u_RoughnessTexToggle", 0.0f);
				if (aiMaterial->GetTexture(aiTextureType_SHININESS, 0, &aiTexPath) == AI_SUCCESS)
				{
					// TODO: Temp - this should be handled by filesystem
					std::filesystem::path path = filename;
					auto parentPath = path.parent_path();
					parentPath /= std::string(aiTexPath.data);
					std::string texturePath = parentPath.string();
					HE_CORE_INFO("    Roughness map path = {0}", texturePath);
					auto texture = Texture2D::Create(texturePath);
					if (texture->Loaded())
					{
						mi->Set("u_RoughnessTexture", texture);
						mi->Set("u_RoughnessTexToggle", 1.0f);
					}
					else
					{
						HE_CORE_ERROR("    Could not load texture: {0}", texturePath);
					}
				}
				else
				{
					HE_CORE_INFO("    No roughness map");
					mi->Set("u_Roughness", roughness);
				}

				bool metalnessTextureFound = false;

				for (uint32_t i = 0; i < aiMaterial->mNumProperties; i++)
				{
					auto prop = aiMaterial->mProperties[i];

#ifdef HE_DEBUG
					/*
					HE_CORE_INFO("Material Property:");
					HE_CORE_INFO("  Name = {0}", prop->mKey.data);
					// HE_CORE_INFO("  Type = {0}", prop->mType);
					// HE_CORE_INFO("  Size = {0}", prop->mDataLength);
					float data = *(float*)prop->mData;
					HE_CORE_INFO("  Value = {0}", data);

					switch (prop->mSemantic)
					{
					case aiTextureType_NONE:
						HE_CORE_INFO("  Semantic = aiTextureType_NONE");
						break;
					case aiTextureType_DIFFUSE:
						HE_CORE_INFO("  Semantic = aiTextureType_DIFFUSE");
						break;
					case aiTextureType_SPECULAR:
						HE_CORE_INFO("  Semantic = aiTextureType_SPECULAR");
						break;
					case aiTextureType_AMBIENT:
						HE_CORE_INFO("  Semantic = aiTextureType_AMBIENT");
						break;
					case aiTextureType_EMISSIVE:
						HE_CORE_INFO("  Semantic = aiTextureType_EMISSIVE");
						break;
					case aiTextureType_HEIGHT:
						HE_CORE_INFO("  Semantic = aiTextureType_HEIGHT");
						break;
					case aiTextureType_NORMALS:
						HE_CORE_INFO("  Semantic = aiTextureType_NORMALS");
						break;
					case aiTextureType_SHININESS:
						HE_CORE_INFO("  Semantic = aiTextureType_SHININESS");
						break;
					case aiTextureType_OPACITY:
						HE_CORE_INFO("  Semantic = aiTextureType_OPACITY");
						break;
					case aiTextureType_DISPLACEMENT:
						HE_CORE_INFO("  Semantic = aiTextureType_DISPLACEMENT");
						break;
					case aiTextureType_LIGHTMAP:
						HE_CORE_INFO("  Semantic = aiTextureType_LIGHTMAP");
						break;
					case aiTextureType_REFLECTION:
						HE_CORE_INFO("  Semantic = aiTextureType_REFLECTION");
						break;
					case aiTextureType_UNKNOWN:
						HE_CORE_INFO("  Semantic = aiTextureType_UNKNOWN");
						break;
					}
					*/
#endif

					if (prop->mType == aiPTI_String)
					{
						uint32_t strLength = *(uint32_t*)prop->mData;
						std::string str(prop->mData + 4, strLength);

						std::string key = prop->mKey.data;
						if (key == "$raw.ReflectionFactor|file")
						{
							metalnessTextureFound = true;

							// TODO: Temp - this should be handled by filesystem
							std::filesystem::path path = filename;
							auto parentPath = path.parent_path();
							parentPath /= str;
							std::string texturePath = parentPath.string();
							HE_CORE_INFO("    Metalness map path = {0}", texturePath);
							auto texture = Texture2D::Create(texturePath);
							if (texture->Loaded())
							{
								mi->Set("u_MetalnessTexture", texture);
								mi->Set("u_MetalnessTexToggle", 1.0f);
							}
							else
							{
								HE_CORE_ERROR("    Could not load texture: {0}", texturePath);
								mi->Set("u_Metalness", metalness);
								mi->Set("u_MetalnessTexToggle", 0.0f);
							}
							break;
						}
					}
				}

				if (!metalnessTextureFound)
				{
					HE_CORE_INFO("    No metalness map");

					mi->Set("u_Metalness", metalness);
					mi->Set("u_MetalnessTexToggle", 0.0f);
				}
			}
			HE_CORE_INFO("------------------------");
		}

		BufferLayout vertexLayout = {
			{ ShaderDataType::Float3, "a_Position" },
			{ ShaderDataType::Float3, "a_Normal" },
			{ ShaderDataType::Float3, "a_Tangent" },
			{ ShaderDataType::Float3, "a_Binormal" },
			{ ShaderDataType::Float2, "a_TexCoord" },
		};

		m_VertexBuffer = VertexBuffer::Create(m_StaticVertices.data(), m_StaticVertices.size() * sizeof(Vertex));
		m_VertexBuffer->Bind();
		m_VertexBuffer->SetLayout(vertexLayout);

		m_IndexBuffer = IndexBuffer::Create((uint32_t*)&m_Indices[0], m_Indices.size() * 3);

		m_VertexArray = VertexArray::Create();
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);
		m_VertexArray->SetIndexBuffer(m_IndexBuffer);
	}

	void Mesh::OnUpdate(Timestep ts)
	{
		/*
		if (m_IsAnimated)
		{
			
		}
		*/
	}
	static std::string LevelToSpaces(uint32_t level)
	{
		std::string result = "";
		for (uint32_t i = 0; i < level; i++)
			result += "--";
		return result;
	}

	void Mesh::TraverseNodes(aiNode* node, const glm::mat4& parentTransform, uint32_t level)
	{
		glm::mat4 transform = parentTransform * Mat4FromAssimpMat4(node->mTransformation);
		for (uint32_t i = 0; i < node->mNumMeshes; i++)
		{
			uint32_t mesh = node->mMeshes[i];
			auto& submesh = m_Submeshes[mesh];
			submesh.NodeName = node->mName.C_Str();
			submesh.Transform = transform;
		}

		for (uint32_t i = 0; i < node->mNumChildren; i++)
			TraverseNodes(node->mChildren[i], transform, level + 1);
	}
}