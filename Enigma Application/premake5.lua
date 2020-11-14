project "Enigma Application"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

	pchheader "pch.hpp"
	pchsource "Source/pch.cpp"

	--warnings "Extra"
	
	files
	{
		"Source/**.h",
		"Source/**.hpp",
		"Source/**.cpp"
	}


	includedirs
	{
		"Source", -- include Source/ dir so we can include pch.hpp directly
		"%{wks.location}/Enigma/Source", -- Inlcude Enigma Sources
		"%{IncludeDir.spdlog}", -- spdlog
		"%{IncludeDir.glfw}", -- GLFW
		"%{IncludeDir.glad}", -- Glad
		"%{IncludeDir.imgui}", -- ImGui
		"%{IncludeDir.pfd}", -- pfd
		"%{IncludeDir.stb_image}", -- pfd
	}

	links
	{
		"Enigma"	-- link Enigma App with Enigma lib
	}


	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"_CRT_SECURE_NO_WARNINGS", -- disable MSVC std warnings
		}

	filter "system:linux"
		defines
		{
		}

	filter "system:macosx"
		defines
		{
		}

	
	--- Configurations ---
	filter "configurations:Debug"
		kind "ConsoleApp" --We need console for debugging
		defines "ENIGMA_DEBUG"
		runtime "Debug"
		symbols "On"
		optimize "Off"
		
	filter "configurations:Release"
		kind "WindowedApp" -- Release as windowed application
		defines "ENIGMA_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		kind "WindowedApp" -- Distribution as windowed application
		defines "ENIGMA_DIST"
		runtime "Release"
		optimize "Full"