#pragma once

#include <string>

namespace Nut {
	class Shader {
	public:
		virtual ~Shader() {}

		virtual void Bind() const = 0;
		virtual void UnBind() const = 0;

		virtual const std::string& GetName() const = 0;

		static Ref<Shader> Create(const std::string& path);
		static Ref<Shader> Create(const std::string& name, const std::string& vertexSrc, const std::string FragmentSrc);
		//virtual void UploadUniformBuffer();
	private:
	};

	class ShaderLibrary {
	public:
		void Add(const Ref<Shader>& s);
		void Add(const std::string& name, const Ref<Shader>& s);
		Ref<Shader> Load(const std::string& path);
		Ref<Shader> Load(const std::string& name, const std::string& path);

		Ref<Shader> Get(const std::string& name);
	private:
		std::unordered_map<std::string, Ref<Shader>> m_Shaders;
	};
}