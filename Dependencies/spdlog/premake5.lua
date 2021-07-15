project "spdlog"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	warnings "Off"


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

	-- Platform --
	-- [[ Windows ]] --
	filter "system:windows"
		systemversion "latest"


	--- Configurations ---
	filter "configurations:Debug"
		runtime "Debug"
		symbols "Full" --vs17 and newer | symbols "On"
		optimize "Off" -- No optimization will be performed.

	filter "configurations:Release"
		runtime "Release"
		optimize "On" -- Perform a balanced set of optimizations.
		inlining "Explicit" -- Only inline functions explicitly marked with the inline keyword.

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full" -- Full optimization.
		inlining "Auto" -- Inline any suitable function for full performance