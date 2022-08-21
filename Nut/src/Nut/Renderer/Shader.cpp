#include "ntpch.h"
#include "Shader.h"

#include "glad/glad.h"


namespace Nut {
	Shader::Shader(const std::string& vertexSrc, const std::string FragmentSrc)
	{
		GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
		const GLchar* source = (const GLchar*)vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);
		glCompileShader(vertexShader);

		GLint isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == false) {
			GLint maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);
			
			glDeleteShader(vertexShader);

			NT_CORE_ERROR("VertexShader compilation failure!");
			NT_CORE_ERROR("{0}", infoLog.data());
			NT_CORE_ASSERT(false, "ShaderConstruction failed, check log!");
			return;
		}

		GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
		source = (const GLchar*)FragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);
		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == false) {
			GLint maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			NT_CORE_ERROR("FragmentShader compilation failure!");
			NT_CORE_ERROR("{0}", infoLog.data());
			NT_CORE_ASSERT(false, "ShaderConstruction failed, check log!");
			return;
		}

		m_RendererID = glCreateProgram();
		glAttachShader(m_RendererID, vertexShader);
		glAttachShader(m_RendererID, fragmentShader);
		glLinkProgram(m_RendererID);

		GLint isLinked = 0;
		glGetProgramiv(m_RendererID, GL_LINK_STATUS, &isLinked);
		if (isLinked == GL_FALSE) {
			GLint maxLength = 0;
			glGetProgramiv(m_RendererID, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<GLchar> infoLog(maxLength);
			glGetProgramInfoLog(m_RendererID, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(m_RendererID);
			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			NT_CORE_ERROR("ShaderProgram linking failure!");
			NT_CORE_ERROR("{0}", infoLog.data());
			NT_CORE_ASSERT(false, "ShaderConstruction failed, check log!");
			return;
		}

		glDetachShader(m_RendererID, vertexShader);
		glDetachShader(m_RendererID, fragmentShader);
	}

	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);
	}

	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);
	}

	void Shader::UnBind() const
	{
		glUseProgram(0);
	}

}
