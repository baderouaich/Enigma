require("premake", ">=5.0.0-alpha15")

include "./Premake/premake_customization/solution_items.lua"

workspace "Enigma"  -- Workspace which is the solution in windows, in xcode will be something else
	architecture "x86_64" -- Targeted architecture x64
	startproject "Enigma" -- Set 'Enigma Application' project as start up project

	configurations
	{
		"Debug", 
		"Release", 
		"Dist" -- Distribution, when everything is done, optimization: Full
	}

	-- include editorconfig, which is a configuration file convention that is
	-- used to define and maintain consistent code styles between team members working
	-- on the same code as well as between different editors and IDEs that they might use.
	-- The styles are saved in INI-like files named
	solution_items
	{
		".editorconfig" -- depends on ./Premake/premake_customization/solution_items.lua
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
IncludeDir["glfw"]			= "%{wks.location}/Dependencies/glfw/include"
IncludeDir["glad"]			= "%{wks.location}/Dependencies/glad/include"
IncludeDir["imgui"]			= "%{wks.location}/Dependencies/imgui" 
IncludeDir["spdlog"]		= "%{wks.location}/Dependencies/spdlog/include" 
IncludeDir["pfd"]			= "%{wks.location}/Dependencies/pfd/include" 
IncludeDir["stb_image"]		= "%{wks.location}/Dependencies/stb_image/include" 
IncludeDir["cryptopp"]		= "%{wks.location}/Dependencies/cryptopp" 
IncludeDir["catch2"]		= "%{wks.location}/Dependencies/catch2/src" 

-- Organize libs solution in a single filtered directory 
group "Dependencies"
	include "Dependencies/glfw" --there is a premake5.lua in Dependencies/glfw/ which will be detected and set things up for us
	include "Dependencies/glad"
	include "Dependencies/imgui"
	include "Dependencies/spdlog"
	include "Dependencies/pfd"
	include "Dependencies/stb_image"
	include "Dependencies/cryptopp"
	include "Dependencies/catch2" -- will be included and linked only in debug mode
group ""

-- Include Enigma Application project solution
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
		"Source", -- include Source/ dir so we can include e.g "Enigma/Core/Core.hpp" directly, not to keep going back steps e.g "../../Core/Core.hpp"
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
	}


	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		--executable icon
	    files { 'resources.rc', '**.ico' }
	    vpaths 
	    {
	  	  ['Resources/*'] = { '*.rc', '**.ico' } 
	    }
		--
		defines
		{
			"ENIGMA_PLATFORM_WINDOWS",
			"_CRT_SECURE_NO_WARNINGS", -- disable MSVC std warnings
		}
		links
		{
			"opengl32" -- Loads windows opengl32 kernel .lib
		}

	filter "system:linux"
		defines
		{
			"ENIGMA_PLATFORM_LINUX",
		}
		links
		{
			"X11", -- x11 Linux gui libs
			"dl", -- dynamic loader interface
			"GL", -- unix based systems opengl lib
			"pthread", -- std::thread
			"stdc++fs" -- std::filesystem
		}

	filter "system:macosx"
		defines
		{
			"ENIGMA_PLATFORM_MACOS",
		}	
		links
		{
			-- TODO
			"GL", -- unix based systems opengl
			"pthread", -- std::thread
			"stdc++fs" -- std::filesystem
		}

	
	--- Configurations ---
	filter "configurations:Debug"
		kind "ConsoleApp" -- Debug we need console for logging
		defines "ENIGMA_DEBUG"
		runtime "Debug"
		symbols "On"
		optimize "Off" -- No optimization will be performed.
		includedirs
		{
			"%{IncludeDir.catch2}", -- we use catch2 only in debug
		}
		links
		{
			"catch2", -- we need catch2 tests only in debug
		}
		
	filter "configurations:Release"
		kind "WindowedApp" -- Release as windowed application
		defines "ENIGMA_RELEASE"
		runtime "Release"
		optimize "On" -- Perform a balanced set of optimizations.
		inlining "Explicit" -- Only inline functions explicitly marked with the inline keyword.


	filter "configurations:Dist"
		kind "WindowedApp" -- Distribution as windowed application
		defines "ENIGMA_DIST"
		runtime "Release"
		optimize "Full" -- Full optimization.
		inlining "Auto" -- Inline any suitable function for full performance

