workspace "Nut"
    startproject "Nero"
    architecture "x64"

    configurations {
        "Debug",
        "Resease",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["GLFW"] = "Nut/vendor/GLFW/include"
IncludeDir["GLAD"] = "Nut/vendor/GLAD/include"
IncludeDir["ImGui"] = "Nut/vendor/imgui"
IncludeDir["glm"] = "Nut/vendor/glm"
IncludeDir["stb"] = "Nut/vendor/stb"
IncludeDir["entt"] = "Nut/vendor/entt"
IncludeDir["yaml"] = "Nut/vendor/yaml-cpp/include"
IncludeDir["ImGuizmo"] = "Nut/vendor/ImGuizmo"

group "Dependencies"
    include "Nut/vendor/GLFW"
    include "Nut/vendor/GLAD"
    include "Nut/vendor/imgui"
    include "Nut/vendor/yaml-cpp"
group ""


project "Nut"
    location "Nut"
    kind "StaticLib"
    language "C++"
    cppdialect "C++17"
    staticruntime"on"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    pchheader "ntpch.h"
    pchsource "Nut/src/ntpch.cpp"

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.inl",
        "%{prj.name}/src/**.c",
        "%{prj.name}/vendor/glm/glm/**.h",
        "%{prj.name}/vendor/glm/glm/**.cpp",
        "%{prj.name}/vendor/glm/glm/**.hpp",
        "%{prj.name}/vendor/glm/glm/**.inl",
        "%{prj.name}/vendor/glm/glm/**.c",
        "%{prj.name}/vendor/stb/**.h",
        "%{prj.name}/vendor/stb/**.cpp",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.h",
        "%{prj.name}/vendor/ImGuizmo/ImGuizmo.cpp",
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include",
        "%{IncludeDir.GLFW}",
        "%{IncludeDir.GLAD}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.stb}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.yaml}",
        "%{IncludeDir.ImGuizmo}"
    }

    links{
        "GLFW",
        "GLAD",
        "ImGui",
        "yaml-cpp",
        "opengl32.lib"
    }

    filter "files:%{prj.name}/vendor/ImGuizmo/**.cpp"
        flags { "NoPCH" }

    filter "system:windows"
        staticruntime "On"
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

    filter "configurations:Release"
        defines "NT_RELEASE"
        runtime "Release"
        optimize "On"

    filter "configurations:Dist"
        defines "NT_DIST"
        runtime "Release"
        optimize "On"



project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"
        staticruntime "On"

        language "C++"
        cppdialect "C++17"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("int/" .. outputdir .. "/%{prj.name}")

        files {
           "%{prj.name}/src/**.h",
           "%{prj.name}/src/**.hpp",
           "%{prj.name}/src/**.cpp",
           "%{prj.name}/src/**.c"
        }

        includedirs {
            "Nut/vendor/spdlog/include",
            "Nut/src",
            "Nut/vendor",
            "%{IncludeDir.yaml}"
        }

        links{
            "Nut",
            "ImGui"
        }
    
        filter "system:windows"
            cppdialect "C++17"
            staticruntime "On"
            systemversion "latest"
    
            defines {
                "NT_PLATFORM_WINDOWS"
            }
    
        filter "configurations:Debug"
            defines "NT_DEBUG"
            runtime "Debug"
            symbols "On"
    
        filter "configurations:Release"
            defines "NT_RELEASE"
            runtime "Release"
            optimize "On"
    
        filter "configurations:Dist"
            defines "NT_DIST"
            runtime "Release"
            optimize "On"






project "Nero"
    location "Nero"
    kind "ConsoleApp"
    staticruntime "On"

    language "C++"
    cppdialect "C++17"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.hpp",
        "%{prj.name}/src/**.c"
    }

    includedirs {
        "Nut/vendor/spdlog/include",
        "Nut/src",
        "Nut/vendor",
        "%{IncludeDir.yaml}"
    }

    links{
        "Nut",
        "ImGui"
    }
    
    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"
    
        defines {
            "NT_PLATFORM_WINDOWS"
        }
    
    filter "configurations:Debug"
        defines "NT_DEBUG"
        runtime "Debug"
        symbols "On"
    
    filter "configurations:Release"
        defines "NT_RELEASE"
        runtime "Release"
        optimize "On"
    
    filter "configurations:Dist"
        defines "NT_DIST"
        runtime "Release"
        optimize "On"