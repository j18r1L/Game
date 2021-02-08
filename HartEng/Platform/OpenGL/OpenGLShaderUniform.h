#pragma once

#include "HartEng/Renderer/ShaderUniform.h"
#include "HartEng/Core/Log.h"

namespace HE
{
	class OpenGLShaderResourceDeclaration : public ShaderResourceDeclaration
	{
	public:
		enum class Type
		{
			NONE, TEXTURE2D, TEXTURECUBE
		};
	private:
		std::string m_Name;
		uint32_t m_Register = 0;
		uint32_t m_Count;
		Type m_Type;

		friend class OpenGLShader;
	public:
		OpenGLShaderResourceDeclaration(Type type, const std::string& name, uint32_t count);

		inline const std::string& GetName() const override { return m_Name; }
		inline uint32_t GetRegister() const override { return m_Register; }
		inline uint32_t GetCount() const override { return m_Count; }
		inline Type GetType() const { return m_Type; }

		static Type StringToType(const std::string& type);
		static std::string TypeToString(Type type);
	};

	class OpenGLShaderUniformDeclaration : public ShaderUniformDeclaration
	{
	public:
		enum class Type
		{
			NONE, FLOAT32, VEC2, VEC3, VEC4, MAT3, MAT4, INT32, BOOL, STRUCT
		};
	private:
		std::string m_Name;
		uint32_t m_Size;
		uint32_t m_Count;
		uint32_t m_Offset;
		ShaderDomain m_Domain;

		Type m_Type;
		ShaderStruct* m_Struct;
		mutable int32_t m_Location;

		friend class OpenGLShaderUniformBufferDeclaration;
		friend class OpenGLShader;

	public:
		OpenGLShaderUniformDeclaration(ShaderDomain domain, Type type, const std::string& name, uint32_t count = 1);
		OpenGLShaderUniformDeclaration(ShaderDomain domain, ShaderStruct* uniformStruct, const std::string& name, uint32_t count = 1);

		const std::string& GetName() const override { return m_Name; }
		uint32_t GetSize() const override { return m_Size; }
		uint32_t GetCount() const override { return m_Count; }
		uint32_t GetOffset() const override { return m_Offset; }
		uint32_t GetAbsoluteOffet() const { return m_Struct ? m_Struct->GetOffset() + m_Offset : m_Offset; }
		ShaderDomain GetDomain() const { return m_Domain; }
		int32_t GetLocation() const { return m_Location; }
		Type GetType() const { return m_Type; }
		const ShaderStruct& GetShaderUniformStruct() const { HE_CORE_ASSERT(m_Struct, "No ShaderUniformStruct is provided!"); return *m_Struct; }

		bool IsArray() const { return m_Count > 1; }

		static uint32_t SizeOfUniformType(Type type);
		static Type StringToType(const std::string& type);
		static std::string TypeToString(Type type);

	protected:
		void SetOffset(uint32_t offset) override;

	};

	struct GLShaderUniformField
	{
		OpenGLShaderUniformDeclaration::Type type;
		std::string name;
		uint32_t count;
		uint32_t size;
		int32_t location;
	};

	class OpenGLShaderUniformBufferDeclaration : public ShaderUniformBufferDeclaration
	{
	private:
		std::string m_Name;
		std::vector<ShaderUniformDeclaration*> m_Uniforms;
		uint32_t m_Register;
		uint32_t m_Size;
		ShaderDomain m_Domain;
	public:
		OpenGLShaderUniformBufferDeclaration(const std::string& name, ShaderDomain domain);

		void PushUniform(OpenGLShaderUniformDeclaration* uniform);

		const std::string& GetName() const override { return m_Name; }
		uint32_t GetRegister() const override { return m_Register; }
		uint32_t GetSize() const override { return m_Size; }
		virtual ShaderDomain GetDomain() const { return m_Domain; }
		const std::vector<ShaderUniformDeclaration*>& GetUniformDeclarations() const override { return m_Uniforms; }

		ShaderUniformDeclaration* FindUniform(const std::string& name);
	};
}