workspace "Enigma"  -- Workspace which is the solution in windows, in xcode will be something else
	architecture "x86_64" -- Targeted architecture x64
	startproject "Enigma" -- Set 'Enigma Application' project as start up project

	configurations
	{
		"Debug", 
		"Release", 
		"Dist" -- Distribution, when everything is done, optimization: Full
	}
	
	--https://github.com/premake/premake-core/wiki/flags
	flags
	{
		"MultiProcessorCompile" -- Speed up compile time, Enable Visual Studio to use multiple compiler processes when building.
	}


--Set up the output directory For multiplatforms, see https://github.com/premake/premake-core/wiki/Tokens
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glfw"]		= "%{wks.location}/Dependencies/glfw/include"
IncludeDir["glad"]		= "%{wks.location}/Dependencies/glad/include"
IncludeDir["imgui"]		= "%{wks.location}/Dependencies/imgui" 
IncludeDir["spdlog"]	= "%{wks.location}/Dependencies/spdlog/include" 
IncludeDir["pfd"]		= "%{wks.location}/Dependencies/pfd/include" 
IncludeDir["stb_image"]	= "%{wks.location}/Dependencies/stb_image/include" 
IncludeDir["cryptopp"]	= "%{wks.location}/Dependencies/cryptopp" 


-- Organize libs solution in a single filtered directory 
group "Dependencies"
	include "Dependencies/glfw" --there is a premake5.lua in Dependencies/glfw/ which will be detected and set things up for us
	include "Dependencies/glad"
	include "Dependencies/imgui"
	include "Dependencies/spdlog"
	include "Dependencies/pfd"
	include "Dependencies/stb_image"
	include "Dependencies/cryptopp"
group ""

-- Include Enigma Application projects solution
project "Enigma"
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
		"Source/**.hpp",
		"Source/**.cpp"
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
		"%{IncludeDir.cryptopp}", -- cryptopp
	}


	links
	{
		"glfw", --links GLFW .lib
		"glad", -- links Glad lib
		"imgui", -- links ImGui lib
		"spdlog", -- links SpdLog lib
		"stb_image", -- links stb_image lib
		"cryptopp", -- links cryptopp lib

		"opengl32" --Loads opengl32 kernel .lib
	}


	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		defines
		{
			"_CRT_SECURE_NO_WARNINGS", -- disable MSVC std warnings
		}
		links
		{
		}

	filter "system:linux"
		defines
		{
		}
		links
		{
			"pthread"
		}

	filter "system:macosx"
		defines
		{
		}	
		links
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