project "Enigma"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "on"

	targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Bin-Intermediate/" .. outputdir .. "/%{prj.name}")
	
	pchheader "Epch.hpp"
	pchsource "Source/Epch.cpp"

	--warnings "Extra"

	files
	{
		"Source/**.h", -- C/C++ header files
		"Source/**.hpp", -- C++ header files
		"Source/**.cpp", -- C++ source files
		"Source/**.c", -- C source files
		"Source/**.inl" -- C/C++ inline files
	}

	defines
	{
		"GLFW_INCLUDE_NONE", --so it will not include any opengl headers when u include glfw to avoid glad & glfw headers conflicts
	}

	--Add Libraries include directories
	includedirs
	{
		"Source", -- include Source/ dir so we can include e.g "Enigma/Core/Core.h" directly, not to keep going back steps e.g "../../Core/Core.h"
		"%{IncludeDir.spdlog}", -- spdlog
		"%{IncludeDir.glfw}", -- GLFW
		"%{IncludeDir.glad}", -- Glad
		"%{IncludeDir.imgui}", -- ImGui
		"%{IncludeDir.pfd}", -- pfd
		"%{IncludeDir.stb_image}", -- stb_image
	}


	links
	{
		"glfw", --links GLFW .lib
		"glad", -- links Glad lib
		"imgui", -- links ImGui lib
		"spdlog", -- links SpdLog lib
		"stb_image", -- links stb_image lib

		"opengl32" --Loads opengl32 kernel .lib
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
		defines "ENIGMA_DEBUG"
		runtime "Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		defines "ENIGMA_RELEASE"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		defines "ENIGMA_DIST"
		runtime "Release"
		optimize "Full"
