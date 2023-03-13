#pragma once

#include "Nut/Core/Buffer.h"

#include <filesystem>

namespace Nut {

	class FileSystem {
	public:
		static Buffer ReadFileBinary(const std::filesystem::path& filepath);
	};
}