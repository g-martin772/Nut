#pragma once
#include <cstdint>
#include <string>
#include <Nut/Core.h>

namespace Nut {
	class Texture {
	public:
		virtual ~Texture() {}

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	class Texture2D : public Texture {
	public:
		virtual ~Texture2D() {}
		static Ref<Texture2D> Create(const std::string& path);
	};
}