#pragma once

#include "HartEng/Core/pch.h"
#include "HartEng/Core/Buffer.h"
#include "HartEng/Renderer/ShaderUniform.h"

#include <glm/glm.hpp>

namespace HE
{
	enum class UniformType
	{
		None = 0,
		Float, Float2, Float3, Float4,
		Matrix3x3, Matrix4x4,
		Int32, Uint32

	};
	struct UniformDecl
	{
		UniformType Type;
		std::ptrdiff_t Offset;
		std::string Name;
	};

	struct UniformBufferBase
	{
		virtual const uint8_t* GetBuffer() const = 0;
		virtual const UniformDecl* GetUniforms() const = 0;
		virtual unsigned int GetUniformCount() const = 0;
	};

    class Shader
    {
    public:
		virtual void Reload() = 0;

		virtual void Bind() = 0;
		virtual uint32_t GetRendererID() const = 0;
		virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) = 0;

		// Temporary while we don't have materials
		virtual void SetFloat(const std::string& name, float value) = 0;
		virtual void SetInt(const std::string& name, int value) = 0;
		virtual void SetBool(const std::string& name, bool value) = 0;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) = 0;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) = 0;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) = 0;
		virtual void SetMat4FromRenderThread(const std::string& name, const glm::mat4& value, bool bind = true) = 0;

		virtual void SetIntArray(const std::string& name, int* values, uint32_t size) = 0;

		virtual const std::string& GetName() const = 0;

		// Represents a complete shader program stored in a single file.
		// in the future this will be an asset object + metadata
		static std::shared_ptr<Shader> Create(const std::string& filepath);
		static std::shared_ptr<Shader> CreateFromString(const std::string& source);

		virtual void SetVSMaterialUniformBuffer(Buffer buffer) = 0;
		virtual void SetFSMaterialUniformBuffer(Buffer buffer) = 0;

		virtual const std::vector<ShaderUniformBufferDeclaration*>& GetVSRendererUniforms() const = 0;
		virtual const std::vector<ShaderUniformBufferDeclaration*>& GetFSRendererUniforms() const = 0;
		virtual bool HasVSMaterialUniformBuffer() const = 0;
		virtual bool HasFSMaterialUniformBuffer() const = 0;
		virtual const ShaderUniformBufferDeclaration& GetVSMaterialUniformBuffer() const = 0;
		virtual const ShaderUniformBufferDeclaration& GetFSMaterialUniformBuffer() const = 0;

		virtual const std::vector<ShaderResourceDeclaration*>& GetResources() const = 0;

		virtual void AddShaderReloadedCallback(const std::function<void()>& callback) = 0;

		// Temporary, before we have an asset manager
		static std::vector<std::shared_ptr<Shader>> s_AllShaders;
    };

    class ShaderLibrary
    {
    private:
        std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;
    public:
        void Add(const std::shared_ptr<Shader> shader);
        void Add(const std::string& name, const std::shared_ptr<Shader> shader);

        std::shared_ptr<Shader> Load(const std::string& path);
        std::shared_ptr<Shader> Load(const std::string& name, const std::string& path);

        std::shared_ptr<Shader> Get(const std::string& name);
        const std::unordered_map<std::string, std::shared_ptr<Shader>> GetShaders() const;

        bool Exists(const std::string& name) const;

    };
}