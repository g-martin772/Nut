#pragma once

#include "ntpch.h"

#include "../vendor/glm/glm/glm.hpp"

namespace Nut {
	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string FragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;

		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	private:
		uint32_t m_RendererID;
	};
}