project "Nero"
    kind "ConsoleApp"
    staticruntime "off"

    language "C++"
    cppdialect "C++17"

    targetdir ("%{wks.location}/bin/" .. outputdir .. "/%{prj.name}")
        objdir ("%{wks.location}/int/" .. outputdir .. "/%{prj.name}")

    files {
        "src/**.h",
        "src/**.cpp",
        "src/**.hpp",
        "src/**.c"
    }

    includedirs {
        "%{wks.location}/Nut/src",
        "%{wks.location}/Nut/vendor",
        "%{IncludeDir.spdlog}",
        "%{IncludeDir.yaml}",
        "%{IncludeDir.glm}",
        "%{IncludeDir.entt}",
        "%{IncludeDir.ImGuizmo}",
        "%{IncludeDir.ImGui}",
        "%{IncludeDir.GLFW}", -- for imgui context, neet to get rid of in the future
        "%{IncludeDir.box2d}",
    }

    links{
        "Nut",
        "ImGui"
    }
    
    filter "system:windows"
        cppdialect "C++17"
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