#pragma once

#include "ntpch.h"

namespace Nut {
	class Shader {
	public:
		Shader(const std::string& vertexSrc, const std::string FragmentSrc);
		~Shader();

		void Bind() const;
		void UnBind() const;
	private:
		uint32_t m_RendererID;
	};
}