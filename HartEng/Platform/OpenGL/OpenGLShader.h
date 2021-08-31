#pragma once

#include "HartEng/Core/pch.h"
#include "HartEng/Renderer/Shader.h"
#include "HartEng/Platform/OpenGL/OpenGLShaderUniform.h"

#include <glm/glm.hpp>
#include <glad/glad.h>

namespace HE
{
	class OpenGLShader : public Shader
	{
	private:
		uint32_t m_RendererID = 0;
		bool m_Loaded = false;
		bool m_IsCompute = false;

		std::string m_Name;
		std::vector<std::string> m_AssetPath;
		std::unordered_map<GLenum, std::string> m_ShaderSource;

		std::vector<std::function<void()>> m_ShaderReloadedCallbacks;

		std::vector<ShaderUniformBufferDeclaration*> m_VSRendererUniformBuffers;
		std::vector<ShaderUniformBufferDeclaration*> m_FSRendererUniformBuffers;
		std::shared_ptr<OpenGLShaderUniformBufferDeclaration> m_VSMaterialUniformBuffer;
		std::shared_ptr<OpenGLShaderUniformBufferDeclaration> m_FSMaterialUniformBuffer;
		std::vector<ShaderResourceDeclaration*> m_Resources;
		std::vector<ShaderStruct*> m_Structs;
	public:
		OpenGLShader() = default;
		OpenGLShader(const std::string& filepath);
		OpenGLShader(const std::string& vertexfilepath, const std::string& fragmentfilepath);
		static std::shared_ptr<OpenGLShader> CreateFromString(const std::string& source);

		virtual void Reload() override;
		virtual void AddShaderReloadedCallback(const std::function<void()>& callback) override;

		virtual void Bind() override;
		virtual uint32_t GetRendererID() const override { return m_RendererID; }

		virtual void UploadUniformBuffer(const UniformBufferBase& uniformBuffer) override;

		virtual void SetVSMaterialUniformBuffer(Buffer buffer) override;
		virtual void SetFSMaterialUniformBuffer(Buffer buffer) override;

		virtual void SetInt(const std::string& name, int value) override;
		virtual void SetBool(const std::string& name, bool value) override;
		virtual void SetFloat(const std::string& name, float value) override;
		virtual void SetFloat2(const std::string& name, const glm::vec2& value) override;
		virtual void SetFloat3(const std::string& name, const glm::vec3& value) override;
		virtual void SetMat4(const std::string& name, const glm::mat4& value) override;
		virtual void SetMat4FromRenderThread(const std::string& name, const glm::mat4& value, bool bind = true) override;

		virtual void SetIntArray(const std::string& name, int* values, uint32_t size) override;

		virtual const std::string& GetName() const override { return m_Name; }
	private:
		void Load(const std::string& source);

		std::string ReadShaderFromFile(const std::string& filepath) const;
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
		void Parse();
		void ParseUniform(const std::string& statement, ShaderDomain domain);
		void ParseUniformStruct(const std::string& block, ShaderDomain domain);
		ShaderStruct* FindStruct(const std::string& name);

		int32_t GetUniformLocation(const std::string& name) const;

		void ResolveUniforms();
		void ValidateUniforms();
		void CompileAndUploadShader();
		static GLenum ShaderTypeFromString(const std::string& type);

		void ResolveAndSetUniforms(const std::shared_ptr<OpenGLShaderUniformBufferDeclaration>& decl, Buffer buffer);
		void ResolveAndSetUniform(OpenGLShaderUniformDeclaration* uniform, Buffer buffer);
		void ResolveAndSetUniformArray(OpenGLShaderUniformDeclaration* uniform, Buffer buffer);
		void ResolveAndSetUniformField(const OpenGLShaderUniformDeclaration& field, byte* data, int32_t offset);

		void UploadUniformInt(uint32_t location, int32_t value);
		void UploadUniformIntArray(uint32_t location, int32_t* values, int32_t count);
		void UploadUniformFloat(uint32_t location, float value);
		void UploadUniformFloat2(uint32_t location, const glm::vec2& value);
		void UploadUniformFloat3(uint32_t location, const glm::vec3& value);
		void UploadUniformFloat4(uint32_t location, const glm::vec4& value);
		void UploadUniformMat3(uint32_t location, const glm::mat3& values);
		void UploadUniformMat4(uint32_t location, const glm::mat4& values);
		void UploadUniformMat4Array(uint32_t location, const glm::mat4& values, uint32_t count);

		void UploadUniformStruct(OpenGLShaderUniformDeclaration* uniform, byte* buffer, uint32_t offset);

		void UploadUniformInt(const std::string& name, int32_t value);
		void UploadUniformIntArray(const std::string& name, int32_t* values, uint32_t count);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		void UploadUniformMat4(const std::string& name, const glm::mat4& value);

		virtual const std::vector<ShaderUniformBufferDeclaration*>& GetVSRendererUniforms() const override { return m_VSRendererUniformBuffers; }
		virtual const std::vector<ShaderUniformBufferDeclaration*>& GetFSRendererUniforms() const override { return m_FSRendererUniformBuffers; }
		virtual bool HasVSMaterialUniformBuffer() const override { return (bool)m_VSMaterialUniformBuffer; }
		virtual bool HasFSMaterialUniformBuffer() const override { return (bool)m_FSMaterialUniformBuffer; }
		virtual const ShaderUniformBufferDeclaration& GetVSMaterialUniformBuffer() const override { return *m_VSMaterialUniformBuffer; }
		virtual const ShaderUniformBufferDeclaration& GetFSMaterialUniformBuffer() const override { return *m_FSMaterialUniformBuffer; }
		virtual const std::vector<ShaderResourceDeclaration*>& GetResources() const override { return m_Resources; }
	

	};
}

