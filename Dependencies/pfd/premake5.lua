project "pfd"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/portable-file-dialogs.h",
		"src/portable-file-dialogs.cpp"
	}

	defines
	{
		
	}

	includedirs
	{
		"include"
	}

	filter "system:windows"
		systemversion "latest"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full"