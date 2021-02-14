#include "HartEng/Renderer/Material.h"

namespace HE
{
	
	std::shared_ptr<Material> Material::Create(const std::shared_ptr<Shader>& shader)
	{
		return std::make_shared<Material>(shader);
	}

	Material::Material(const std::shared_ptr<Shader>& shader): 
		m_Shader(shader)
	{
		m_Shader->AddShaderReloadedCallback(std::bind(&Material::OnShaderReloaded, this));
		AllocateStorage();

		m_MaterialFlags |= (uint32_t)MaterialFlag::DepthTest;
		m_MaterialFlags |= (uint32_t)MaterialFlag::Blend;
	}

	Material::~Material()
	{
	}

	void Material::AllocateStorage()
	{
		if (m_Shader->HasVSMaterialUniformBuffer())
		{
			const auto& vsBuffer = m_Shader->GetVSMaterialUniformBuffer();
			m_VSUniformStorageBuffer.Allocate(vsBuffer.GetSize());
			m_VSUniformStorageBuffer.ZeroInitialize();
		}

		if (m_Shader->HasFSMaterialUniformBuffer())
		{
			const auto& psBuffer = m_Shader->GetFSMaterialUniformBuffer();
			m_FSUniformStorageBuffer.Allocate(psBuffer.GetSize());
			m_FSUniformStorageBuffer.ZeroInitialize();
		}
	}

	void Material::OnShaderReloaded()
	{
		return;
		AllocateStorage();

		for (auto mi : m_MaterialInstances)
			mi->OnShaderReloaded();
	}

	ShaderUniformDeclaration* Material::FindUniformDeclaration(const std::string& name)
	{
		if (m_VSUniformStorageBuffer)
		{
			auto& declarations = m_Shader->GetVSMaterialUniformBuffer().GetUniformDeclarations();
			for (ShaderUniformDeclaration* uniform : declarations)
			{
				if (uniform->GetName() == name)
					return uniform;
			}
		}

		if (m_FSUniformStorageBuffer)
		{
			auto& declarations = m_Shader->GetFSMaterialUniformBuffer().GetUniformDeclarations();
			for (ShaderUniformDeclaration* uniform : declarations)
			{
				if (uniform->GetName() == name)
					return uniform;
			}
		}
		return nullptr;
	}

	ShaderResourceDeclaration* Material::FindResourceDeclaration(const std::string& name)
	{
		auto& resources = m_Shader->GetResources();
		for (ShaderResourceDeclaration* resource : resources)
		{
			if (resource->GetName() == name)
				return resource;
		}
		return nullptr;
	}

	Buffer& Material::GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration)
	{
		switch (uniformDeclaration->GetDomain())
		{
		case ShaderDomain::Vertex:    return m_VSUniformStorageBuffer;
		case ShaderDomain::Fragment:     return m_FSUniformStorageBuffer;
		}

		HE_CORE_ASSERT(false, "Invalid uniform declaration domain! Material does not support this shader type.");
		return m_VSUniformStorageBuffer;
	}

	void Material::Bind()
	{
		m_Shader->Bind();

		if (m_VSUniformStorageBuffer)
			m_Shader->SetVSMaterialUniformBuffer(m_VSUniformStorageBuffer);

		if (m_FSUniformStorageBuffer)
			m_Shader->SetFSMaterialUniformBuffer(m_FSUniformStorageBuffer);

		BindTextures();
	}

	void Material::BindTextures()
	{
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			auto& texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}

	//////////////////////////////////////////////////////////////////////////////////
	// MaterialInstance
	//////////////////////////////////////////////////////////////////////////////////

	std::shared_ptr<MaterialInstance> MaterialInstance::Create(const std::shared_ptr<Material>& material)
	{
		return std::make_shared<MaterialInstance>(material);
	}

	MaterialInstance::MaterialInstance(const std::shared_ptr<Material>& material, const std::string& name):
		m_Material(material), m_Name(name)
	{
		m_Material->m_MaterialInstances.insert(this);
		AllocateStorage(); 
	}

	MaterialInstance::~MaterialInstance()
	{
		m_Material->m_MaterialInstances.erase(this);
	}

	void MaterialInstance::OnShaderReloaded()
	{
		AllocateStorage();
		m_OverriddenValues.clear();
	}

	void MaterialInstance::AllocateStorage()
	{
		if (m_Material->m_Shader->HasVSMaterialUniformBuffer())
		{
			const auto& vsBuffer = m_Material->m_Shader->GetVSMaterialUniformBuffer();
			m_VSUniformStorageBuffer.Allocate(vsBuffer.GetSize());
			memcpy(&m_VSUniformStorageBuffer.Data[0], &m_Material->m_VSUniformStorageBuffer.Data[0], vsBuffer.GetSize());
		}

		if (m_Material->m_Shader->HasFSMaterialUniformBuffer())
		{
			const auto& psBuffer = m_Material->m_Shader->GetFSMaterialUniformBuffer();
			m_FSUniformStorageBuffer.Allocate(psBuffer.GetSize());
			memcpy(&m_FSUniformStorageBuffer.Data[0], &m_Material->m_FSUniformStorageBuffer.Data[0], psBuffer.GetSize());
		}
	}

	void MaterialInstance::SetFlag(MaterialFlag flag, bool value)
	{
		if (value)
		{
			m_Material->m_MaterialFlags |= (uint32_t)flag;
		}
		else
		{
			m_Material->m_MaterialFlags &= ~(uint32_t)flag;
		}
	}

	void MaterialInstance::OnMaterialValueUpdated(ShaderUniformDeclaration* decl)
	{
		if (m_OverriddenValues.find(decl->GetName()) == m_OverriddenValues.end())
		{
			auto& buffer = GetUniformBufferTarget(decl);
			auto& materialBuffer = m_Material->GetUniformBufferTarget(decl);
			buffer.Write(&materialBuffer.Data[0] + decl->GetOffset(), decl->GetSize(), decl->GetOffset());
		}
	}

	Buffer& MaterialInstance::GetUniformBufferTarget(ShaderUniformDeclaration* uniformDeclaration)
	{
		switch (uniformDeclaration->GetDomain())
		{
		case ShaderDomain::Vertex:    return m_VSUniformStorageBuffer;
		case ShaderDomain::Fragment:  return m_FSUniformStorageBuffer;
		}

		HE_CORE_ASSERT(false, "Invalid uniform declaration domain! Material does not support this shader type.");
		return m_VSUniformStorageBuffer;
	}

	void MaterialInstance::Bind()
	{
		m_Material->m_Shader->Bind();

		if (m_VSUniformStorageBuffer)
			m_Material->m_Shader->SetVSMaterialUniformBuffer(m_VSUniformStorageBuffer);

		if (m_FSUniformStorageBuffer)
			m_Material->m_Shader->SetFSMaterialUniformBuffer(m_FSUniformStorageBuffer);

		m_Material->BindTextures();
		for (size_t i = 0; i < m_Textures.size(); i++)
		{
			auto& texture = m_Textures[i];
			if (texture)
				texture->Bind(i);
		}
	}
}