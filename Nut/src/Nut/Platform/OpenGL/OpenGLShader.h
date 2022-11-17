#pragma once
#include <cstdint>
#include <string>
#include <Nut/Renderer/Shader.h>

typedef unsigned int GLenum;

namespace Nut {
	class OpenGLShader : public Shader{
	public:
		OpenGLShader(const std::string& path);
		OpenGLShader(const std::string& name, const std::string& vertexSrc, const std::string FragmentSrc);
		~OpenGLShader();

		const std::string& GetName() const override { return m_Name; }

		void Bind() const override;
		void UnBind() const override;

		void SetInt(const std::string& name, int value) override;
		void SetIntArr(const std::string& name, int* values, uint32_t count) override;
		void SetFloat(const std::string& name, float value) override;
		void SetFloat3(const std::string& name, const glm::vec3& value) override;
		void SetFloat4(const std::string& name, const glm::vec4& value) override;
		void SetMat4(const std::string& name, const glm::mat4& value) override;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat1(const std::string& name, const glm::vec1& values);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArr(const std::string& name, int* values, uint32_t count);
		void UploadUniformInt1(const std::string& name, const glm::vec1& values);
		void UploadUniformInt2(const std::string& name, const glm::vec2& values);
		void UploadUniformInt3(const std::string& name, const glm::vec3& values);
		void UploadUniformInt4(const std::string& name, const glm::vec4& values);
	private:
		std::string ReadShaderFile(const std::string& path);
		std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);

		void CompileOrGetVulkanBinaries(const std::unordered_map<GLenum, std::string>& shaderSources);
		void CompileOrGetOpenGLBinaries();
		void CreateProgram();
		void Reflect(GLenum stage, const std::vector<uint32_t>& shaderData);
	private:
		uint32_t m_RendererID;
		std::string m_Name;

		std::string m_FilePath;

		std::unordered_map<GLenum, std::vector<uint32_t>> m_VulkanSPIRV;
		std::unordered_map<GLenum, std::vector<uint32_t>> m_OpenGLSPIRV;

		std::unordered_map<GLenum, std::string> m_OpenGLSourceCode;
	};
}