#pragma once

#include "HartEng/Renderer/VertexArray.h"
#include "HartEng/Renderer/Material.h"
#include "HartEng/Renderer/Texture.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Core/Timestep.h"
#include "HartEng/Asset/Assets.h"
#include "HartEng/Core/AABB.h"

#include <GLM/glm.hpp>
#include <assimp/Importer.hpp>

#include <iostream>
#include <vector>
#include <unordered_map>

struct aiNode;
struct aiAnimation;
struct aiNodeAnim;
struct aiScene;

namespace Assimp {
	class Importer;
}

namespace HE
{
	struct Vertex
	{
		glm::vec3 Position;
		glm::vec3 Normal;
		glm::vec3 Tangent;
		glm::vec3 Binormal;
		glm::vec2 Texcoord;
	};

	static const int NumAttributes = 5;

	struct Index
	{
		uint32_t V1, V2, V3;
	};

	static_assert(sizeof(Index) == 3 * sizeof(uint32_t));

	struct Triangle
	{
		Vertex V0, V1, V2;

		Triangle(const Vertex& v0, const Vertex& v1, const Vertex& v2)
			: V0(v0), V1(v1), V2(v2) {}
	};

	class Submesh
	{
	public:
		uint32_t BaseVertex;
		uint32_t BaseIndex;
		uint32_t MaterialIndex;
		uint32_t IndexCount;
		uint32_t VertexCount;

		glm::mat4 Transform;
		glm::mat4 LocalTransform;

		AABB aabb;

		std::string NodeName, MeshName;
	};

	class Mesh: public Asset
	{
	public:
		Mesh(const std::string& filename, bool flippedTextures = false);
		Mesh(const std::vector<Vertex>& vertices, const std::vector<Index>& indices, const glm::mat4& transform);
		virtual ~Mesh() = default;

		void OnUpdate(Timestep ts);

		std::vector<Submesh>& GetSubmeshes() { return m_Submeshes; }
		const std::vector<Submesh>& GetSubmeshes() const { return m_Submeshes; }

		const AABB& GetAABB() const { return m_AABB; }

		const std::vector<Vertex>& GetStaticVertices() const { return m_StaticVertices; }
		const std::vector<Index>& GetIndices() const { return m_Indices; }

		std::shared_ptr<Shader> GetMeshShader() { return m_MeshShader; }
		std::shared_ptr<Material> GetMaterial() { return m_BaseMaterial; }
		std::vector<std::shared_ptr<MaterialInstance>> GetMaterials() { return m_Materials; }
		const std::vector<std::shared_ptr<Texture2D>>& GetTextures() const { return m_Textures; }
		const std::string& GetFilePath() const { return m_FilePath; }

		//const std::vector<Triangle> GetTriangleCache(uint32_t index) const { return m_TriangleCache.at(index); }
	private:
		void TraverseNodes(aiNode* node, const glm::mat4& parentTransform = glm::mat4(1.0f), uint32_t level = 0);

	private:
		std::vector<Submesh> m_Submeshes;

		std::unique_ptr<Assimp::Importer> m_Importer;

		glm::mat4 m_InverseTransform;

		AABB m_AABB;

		std::shared_ptr<VertexArray> m_VertexArray;
		std::shared_ptr<VertexBuffer> m_VertexBuffer;
		std::shared_ptr<IndexBuffer> m_IndexBuffer;

		std::vector<Vertex> m_StaticVertices;
		std::vector<Index> m_Indices;
		const aiScene* m_Scene;

		// Materials
		std::shared_ptr<Shader> m_MeshShader;
		std::shared_ptr<Material> m_BaseMaterial;
		std::vector<std::shared_ptr<Texture2D>> m_Textures;
		std::vector<std::shared_ptr<Texture2D>> m_NormalMaps;
		std::vector<std::shared_ptr<MaterialInstance>> m_Materials;

		//std::unordered_map<uint32_t, std::vector<Triangle>> m_TriangleCache;

		std::string m_FilePath;

		friend class Renderer;
		friend class SceneHierarchyPanel;
	};

}