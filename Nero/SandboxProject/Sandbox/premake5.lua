project "Sandbox"
    kind "SharedLib"
    language "C#"
	dotnetframework "4.7.2"

	configurations {
        "Debug",
        "Release",
        "Dist"
    }

    targetdir ("../bin/%{prj.name}")
    objdir ("../int/%{prj.name}")

    files {
        "src/**.cs",
    }

    links {
        "../bin/%{prj.name}/Nut-ScriptCore.dll"
    }

	filter "configurations:Debug"
		optimize "Off"
		symbols "Default"

	filter "configurations:Release"
		optimize "On"
		symbols "Default"

	filter "configurations:Dist"
		optimize "Full"
		symbols "Off"
