#pragma once
#include <cstdint>
#include <string>
#include <../vendor/glm/glm/glm.hpp>
#include <Nut/Renderer/Shader.h>

namespace Nut {
	class OpenGLShader : public Shader{
	public:
		OpenGLShader(const std::string& vertexSrc, const std::string FragmentSrc);
		~OpenGLShader();

		void Bind() const override;
		void UnBind() const override;

		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);

		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat1(const std::string& name, const glm::vec1& values);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& values);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& values);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& values);

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformInt1(const std::string& name, const glm::vec1& values);
		void UploadUniformInt2(const std::string& name, const glm::vec2& values);
		void UploadUniformInt3(const std::string& name, const glm::vec3& values);
		void UploadUniformInt4(const std::string& name, const glm::vec4& values);
	private:
		uint32_t m_RendererID;
	};
}