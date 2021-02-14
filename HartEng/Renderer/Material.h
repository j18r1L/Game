#pragma once

#include <unordered_set>

#include "HartEng/Renderer/Shader.h"
#include "HartEng/Renderer/Texture.h"
#include "HartEng/Core/Buffer.h"

namespace HE
{
	enum class MaterialFlag
	{
		None = BIT(0),
		DepthTest = BIT(1),
		Blend = BIT(2),
		TwoSided = BIT(3)
	};


	class Material
	{
		friend class MaterialInstance;
	private:
		std::shared_ptr<Shader> m_Shader;
		std::unordered_set<MaterialInstance*> m_MaterialInstances;

		Buffer m_VSUniformStorageBuffer;
		Buffer m_FSUniformStorageBuffer;
		std::vector<std::shared_ptr<Texture>> m_Textures;

		uint32_t m_MaterialFlags;
	public:
		Material(const std::shared_ptr<Shader>& shader);
		virtual ~Material();
		static std::shared_ptr<Material> Create(const std::shared_ptr<Shader>& shader);

		void Bind();

		uint32_t GetFlags() const { return m_MaterialFlags; }
		void SetFlag(MaterialFlag flag) { m_MaterialFlags |= (uint32_t)flag; }

		std::shared_ptr<Shader> GetShader() { return m_Shader; }

		template <typename T>
		void Set(const std::string& name, const T& value)
		{
			auto decl = FindUniformDeclaration(name);
			HE_CORE_ASSERT(decl, "Could not find uniform with name " + name);
			auto& buffer = GetUniformBufferTarget(decl);
			buffer.Write((byte*)&value, decl->GetSize(), decl->GetOffset());

			for (auto mi : m_MaterialInstances)
				mi->OnMaterialValueUpdated(decl);
		}

		void Set(const std::string& name, const std::shared_ptr<Texture>& texture)
		{
			auto decl = FindResourceDeclaration(name);
			uint32_t slot = decl->GetRegister();
			if (m_Textures.size() <= slot)
				m_Textures.resize((size_t)slot + 1);
			m_Textures[slot] = texture;
		}

		void Set(const std::string& name, const std::shared_ptr<Texture2D>& texture)
		{
			Set(name, (const std::shared_ptr<Texture>&)texture);
		}

		void Set(const std::string& name, const std::shared_ptr<TextureCube>& texture)
		{
			Set(name, (const std::shared_ptr<Texture>&)texture);
		}

		template<typename T>
		T& Get(const std::string& name)
		{
			auto decl = FindUniformDeclaration(name);
			HE_CORE_ASSERT(decl, "Could not find uniform with name 'x'");
			auto& buffer = GetUniformBufferTarget(decl);
			return buffer.Read<T>(decl->GetOffset());
		}

		template<typename T>
		std::shared_ptr<T> GetResource(const std::string& name)
		{
			auto decl = FindResourceDeclaration(name);
			uint32_t slot = decl->GetRegister();
			HE_CORE_ASSERT(slot < m_Textures.size(), "Texture slot is invalid!");
			return m_Textures[slot];
		}

		ShaderResourceDeclaration* FindResourceDeclaration(const std::string& name);
		
	private:
		void AllocateStorage();
		void OnShaderReloaded();
		void BindTextures();

		ShaderUniformDeclaration* FindUniformDeclaration(const std::string& name);
		Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
	
	};

	class MaterialInstance
	{
		friend class Material;
	private:
		std::shared_ptr<Material> m_Material;
		std::string m_Name;

		Buffer m_VSUniformStorageBuffer;
		Buffer m_FSUniformStorageBuffer;
		std::vector<std::shared_ptr<Texture>> m_Textures;

		// TODO: This is temporary; come up with a proper system to track overrides
		std::unordered_set<std::string> m_OverriddenValues;
	public:
		MaterialInstance(const std::shared_ptr<Material>& material, const std::string& name = "");
		virtual ~MaterialInstance();

		template <typename T>
		void Set(const std::string& name, const T& value)
		{
			auto decl = m_Material->FindUniformDeclaration(name);
			if (!decl)
				return;

			HE_CORE_ASSERT(decl, "Could not find uniform with name 'x'");
			auto& buffer = GetUniformBufferTarget(decl);
			buffer.Write((byte*)&value, decl->GetSize(), decl->GetOffset());

			m_OverriddenValues.insert(name);
		}

		void Set(const std::string& name, const std::shared_ptr<Texture>& texture)
		{
			auto decl = m_Material->FindResourceDeclaration(name);
			if (!decl)
			{
				HE_CORE_WARN("Cannot find material property: ", name);
				return;
			}
			uint32_t slot = decl->GetRegister();
			if (m_Textures.size() <= slot)
				m_Textures.resize((size_t)slot + 1);
			m_Textures[slot] = texture;
		}

		void Set(const std::string& name, const std::shared_ptr<Texture2D>& texture)
		{
			Set(name, (const std::shared_ptr<Texture>&)texture);
		}

		void Set(const std::string& name, const std::shared_ptr<TextureCube>& texture)
		{
			Set(name, (const std::shared_ptr<Texture>&)texture);
		}

		template<typename T>
		T& Get(const std::string& name)
		{
			auto decl = m_Material->FindUniformDeclaration(name);
			HE_CORE_ASSERT(decl, "Could not find uniform with name 'x'");
			auto& buffer = GetUniformBufferTarget(decl);
			return buffer.Read<T>(decl->GetOffset());
		}

		template<typename T>
		std::shared_ptr<T> GetResource(const std::string& name)
		{
			auto decl = m_Material->FindResourceDeclaration(name);
			HE_CORE_ASSERT(decl, "Could not find uniform with name 'x'");
			uint32_t slot = decl->GetRegister();
			HE_CORE_ASSERT(slot < m_Textures.size(), "Texture slot is invalid!");
			return Ref<T>(m_Textures[slot]);
		}

		template<typename T>
		std::shared_ptr<T> TryGetResource(const std::string& name)
		{
			auto decl = m_Material->FindResourceDeclaration(name);
			if (!decl)
				return nullptr;

			uint32_t slot = decl->GetRegister();
			if (slot >= m_Textures.size())
				return nullptr;

			return Ref<T>(m_Textures[slot]);
		}


		void Bind();

		uint32_t GetFlags() const { return m_Material->m_MaterialFlags; }
		bool GetFlag(MaterialFlag flag) const { return (uint32_t)flag & m_Material->m_MaterialFlags; }
		void SetFlag(MaterialFlag flag, bool value = true);

		std::shared_ptr<Shader> GetShader() { return m_Material->m_Shader; }

		const std::string& GetName() const { return m_Name; }

		static std::shared_ptr<MaterialInstance> Create(const std::shared_ptr<Material>& material);
	private:
		void AllocateStorage();
		void OnShaderReloaded();
		Buffer& GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration);
		void OnMaterialValueUpdated(ShaderUniformDeclaration* decl);
	


	};
}