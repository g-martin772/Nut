#pragma once

namespace Nut {
	enum class RendererAPI
	{
		None = 0,
		OpenGL = 1,
		Vulcan = 2,
		Medal = 3,
		DirectX9 = 9, 
		DirextX10 = 10,
		DirectX11 = 11,
		DirectX12 = 12
	};


	class Renderer {
	public:
		Renderer();
		inline static RendererAPI GetAPI() { return s_RendererAPI; }
	private:
		static RendererAPI s_RendererAPI;
	};

}
