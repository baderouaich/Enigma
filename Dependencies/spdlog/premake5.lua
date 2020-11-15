project "spdlog"
	kind "StaticLib"
	language "C++"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	files
	{
		"include/spdlog/**.h",
		"src/spdlog.cpp",
		"src/async.cpp",
		"src/cfg.cpp",
		"src/color_sinks.cpp",
		"src/file_sinks.cpp",
		"src/fmt.cpp",
		"src/stdout_sinks.cpp"
	}
	
	defines
	{
		"SPDLOG_COMPILED_LIB"
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