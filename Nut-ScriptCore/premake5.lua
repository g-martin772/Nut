project "Nut-ScriptCore"
    kind "SharedLib"
    language "C#"
	dotnetframework "4.7.2"

    targetdir ("../Nero/Assets/Scripts/%{prj.name}")
    objdir ("../Nero/Assets/Scripts/int/%{prj.name}")

    files {
        "src/**.cs",
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