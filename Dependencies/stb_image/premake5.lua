project "stb_image"
	kind "StaticLib"
	language "C"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/stb_image.h",
		"src/stb_image.c"
	}
	
	defines
	{
		"STB_IMAGE_STATIC"
	}

	includedirs
	{
		"include"
	}
		
	filter "system:windows"
		systemversion "latest"


	filter "configurations:Debug"
		runtime "Debug"
		symbols "on"

	filter "configurations:Release"
		runtime "Release"
		optimize "on"