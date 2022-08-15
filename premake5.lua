workspace "Nut"
    architecture "x64"

    configurations {
        "Debug",
        "Resease",
        "Dist"
    }

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

project "Nut"
    location "Nut"
    kind "SharedLib"
    language "C++"

    targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("int/" .. outputdir .. "/%{prj.name}")

    files {
        "%{prj.name}/src/**.h",
        "%{prj.name}/src/**.cpp",
        "%{prj.name}/src/**.c"
    }

    includedirs {
        "%{prj.name}/src",
        "%{prj.name}/vendor/spdlog/include"
    }

    filter "system:windows"
        cppdialect "C++17"
        staticruntime "On"
        systemversion "latest"

        defines {
            "NT_PLATFORM_WINDOWS",
            "NT_BUILD_DLL",
            "_WINDLL"
        }

        postbuildcommands{
            ("{COPY} %{cfg.buildtarget.relpath} ../bin/" .. outputdir .. "/Sandbox")
        }

    filter "configurations:Debug"
        defines "NT_DEBUG"
        symbols "On"

    filter "configurations:Release"
        defines "NT_Release"
        optimize "On"

    filter "configurations:Dist"
        defines "NT_Dist"
        optimize "On"



project "Sandbox"
        location "Sandbox"
        kind "ConsoleApp"

        language "C++"

        targetdir ("bin/" .. outputdir .. "/%{prj.name}")
        objdir ("int/" .. outputdir .. "/%{prj.name}")

        files {
           "%{prj.name}/src/**.h",
           "%{prj.name}/src/**.cpp",
           "%{prj.name}/src/**.c"
        }

        includedirs {
            "Nut/vendor/spdlog/include",
            "Nut/src"
        }

        links{
            "Nut"
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
            symbols "On"
    
        filter "configurations:Release"
            defines "NT_Release"
            optimize "On"
    
        filter "configurations:Dist"
            defines "NT_Dist"
            optimize "On"