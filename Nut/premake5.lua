project "Nut"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime "off"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
    objdir ("%{wks.location}/int/" .. outputdir .. "/%{prj.name}")

    pchheader "ntpch.h"
    pchsource "src/ntpch.cpp"

    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.hpp",
        "src/**.inl",
        "src/**.c",
        "vendor/glm/glm/**.h",
        "vendor/glm/glm/**.cpp",
        "vendor/glm/glm/**.hpp",
        "vendor/glm/glm/**.inl",
        "vendor/glm/glm/**.c",
        "vendor/stb/**.h",
        "vendor/stb/**.cpp",
        "vendor/ImGuizmo/ImGuizmo.h",
        "vendor/ImGuizmo/ImGuizmo.cpp",
    }

    includedirs {
        "src",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.VulkanSDK}",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.SPIRV_Cross}",
        "%{IncludeDir.shaderc}",
        "%{IncludeDir.box2d}",
    }

    links{
        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }

    filter "files:vendor/ImGuizmo/**.cpp"
	flags { "NoPCH" }

    filter "files:vendor/glm/glm/**.cpp"
    flags { "NoPCH" }

    filter "files:vendor/glm/glm/**.hpp"
    flags { "NoPCH" }

    filter "system:windows"
        systemversion "latest"

        defines {
            "NT_PLATFORM_WINDOWS",
            "NT_BUILD_DLL",
            "NT_OPENGL",
            "_WINDLL"
        }

    filter "configurations:Debug"
        defines "NT_DEBUG"
        runtime "Debug"
        symbols "On"

        links {
			"%{Library.ShaderC_Debug}",
			"%{Library.SPIRV_Cross_Debug}",
			"%{Library.SPIRV_Cross_GLSL_Debug}"
		}

    filter "configurations:Release"
        defines "NT_RELEASE"
        runtime "Release"
        optimize "On"

        links {
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}

    filter "configurations:Dist"
        defines "NT_DIST"
        runtime "Release"
        optimize "On"

        links {
			"%{Library.ShaderC_Release}",
			"%{Library.SPIRV_Cross_Release}",
			"%{Library.SPIRV_Cross_GLSL_Release}"
		}