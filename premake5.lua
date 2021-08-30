require("premake", ">=5.0.0-alpha15") -- allow premake 5.0.0 alpha15 and up only 

include "./Premake/premake_customization/solution_items.lua"

workspace "Enigma"  -- Workspace which is the solution in windows
	architecture "x86_64" -- Targeted architecture x64
	startproject "Enigma" -- Set 'Enigma' project as start up project
	
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
		"MultiProcessorCompile", -- Speed up compile time, Enable Visual Studio to use multiple compiler processes when building.
		"FatalWarnings" -- Treat all warnings as errors; equivalent to FatalCompileWarnings, FatalLinkWarnings	
	}


--Set up the output directory For multiplatforms, see https://github.com/premake/premake-core/wiki/Tokens
outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

-- Include directories relative to root folder (solution directory)
IncludeDir = {}
IncludeDir["glfw"]			= "%{wks.location}/Dependencies/glfw/include"  -- Graphics Library Framework lib
IncludeDir["glad"]			= "%{wks.location}/Dependencies/glad/include"-- OpenGL lib
IncludeDir["imgui"]			= "%{wks.location}/Dependencies/imgui" 	-- GUI lib
IncludeDir["spdlog"]		= "%{wks.location}/Dependencies/spdlog/include" -- Logging lib
IncludeDir["pfd"]			= "%{wks.location}/Dependencies/pfd/include"  -- portable-file-dialog lib
IncludeDir["stb_image"]		= "%{wks.location}/Dependencies/stb_image/include" -- image loading lib
IncludeDir["cryptopp"]		= "%{wks.location}/Dependencies/cryptopp"  -- Cryptography lib 
IncludeDir["cxxopts"]		= "%{wks.location}/Dependencies/cxxopts/include" -- Arguments parser lib
IncludeDir["inih"]			= "%{wks.location}/Dependencies/inih/include" -- ini config file loader lib
IncludeDir["json"]			= "%{wks.location}/Dependencies/json/include" -- nlohmann json lib
IncludeDir["sqlitecpp"]		= "%{wks.location}/Dependencies/sqlitecpp/include" -- SQLite3 cpp wrapper
IncludeDir["tabulate"]		= "%{wks.location}/Dependencies/tabulate/include" -- Commande line Table maker lib for CLI
IncludeDir["infoware"]		= "%{wks.location}/Dependencies/infoware/include" -- C++ Library for pulling system and hardware information, without hitting the command line.

IncludeDir["curl"]			= "%{wks.location}/Dependencies/curl/include" -- curl networking lib
IncludeDir["cpr"]			= "%{wks.location}/Dependencies/cpr/include" -- Networking library wrapper around cURL (will include and link to curl)
IncludeDir["mbedtls"]		= "%{wks.location}/Dependencies/mbedtls/include" -- cURL depends on mbedtls lib (will be included and linked in curl project for linux, see Dependencies/curl/premake.lua)
IncludeDir["zlib"]			= "%{wks.location}/Dependencies/zlib" -- cURL a depend on it on Linux (will be included and linked in curl project for linux, see Dependencies/curl/premake.lua)

IncludeDir["catch2"]		= "%{wks.location}/Dependencies/catch2/src"  -- Catch2 unit tests

-- Organize libs solution in a single filtered directory 
group "Dependencies"
	include "Dependencies/glfw" -- There is a premake5.lua in Dependencies/glfw/ which will be detected and set things up for us
	include "Dependencies/glad"
	include "Dependencies/imgui"
	include "Dependencies/spdlog"
	include "Dependencies/pfd"
	include "Dependencies/stb_image"
	include "Dependencies/cryptopp"
	include "Dependencies/cxxopts"
	include "Dependencies/inih"
	include "Dependencies/json"
	include "Dependencies/sqlitecpp"
	include "Dependencies/tabulate"
	include "Dependencies/infoware"

	include "Dependencies/curl"
	include "Dependencies/cpr"
	include "Dependencies/mbedtls"-- (will be included and linked in curl lib project for Linux only)
	include "Dependencies/zlib" -- (will be included and linked in curl lib project for Linux only)

	include "Dependencies/catch2" -- will be included and linked only in debug mode
group ""

-- Include Enigma Application project solution
project "Enigma"
	language "C++"
	cppdialect "C++17" -- TODO C++20 using "C++latest"
	staticruntime "on"

	---[[ Binary Output dirs ]]---
	targetdir ("%{wks.location}/Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("%{wks.location}/Bin-Intermediate/" .. outputdir .. "/%{prj.name}")
	--debugdir ("%{wks.location}/")

	---[[ Enable the toolset's maximum warning level (also using fatal warnings flags or -Werror)]]---
	warnings "Extra"

	--[[ 
		Command to copy Resources/ folder next to Enigma.exe (targetdir) after each build https://github.com/premake/premake-core/wiki/postbuildcommands 
	 	Why this?: when Enigma is deployed, we assume Resources/ folder to always be next to the Enigma.exe so it works properly in debug and distribution
    --]]
 	postbuildmessage "Copying ./Resources dir to %{cfg.targetdir}/ ..."
	postbuildcommands {
        { "{COPY} ./Resources %{cfg.targetdir}/Resources" } -- copy Resources/** to %{cfg.targetdir}/Resources
    }			


	---[[ Precompiled Header ]]---
	pchheader "pch.hpp"
	pchsource "Source/pch.cpp"

	---[[ Disable RTTI to optimize if we're not using dynamic_cast or typeid ]]---
	--rtti "Off"
	--exceptionhandling "Off"

	---[[ Source files ]]---
	files
	{
		"Resources/**.*",

		"premake5.lua",
		"Source/**.hpp",
		"Source/**.cpp"
	}

	---[[ Add Libraries include directories ]]---	
	includedirs
	{
		"Source", -- include Source/ dir so we can include e.g "Core/Core.hpp" directly, not to keep going back steps e.g "../../Core/Core.hpp"
		"%{IncludeDir.glfw}",
		"%{IncludeDir.glad}",
		"%{IncludeDir.imgui}",
		"%{IncludeDir.spdlog}",
		"%{IncludeDir.pfd}",
		"%{IncludeDir.stb_image}",
		"%{IncludeDir.cxxopts}",
		"%{IncludeDir.cryptopp}",
		"%{IncludeDir.inih}",
		"%{IncludeDir.json}",
		"%{IncludeDir.sqlitecpp}",
		"%{IncludeDir.curl}",
		"%{IncludeDir.cpr}",
		--"%{IncludeDir.zlib}",
		"%{IncludeDir.tabulate}",
		"%{IncludeDir.infoware}",
	}


	---[[ Link Libraries (Order Matters) ]]---
	links
	{
		"glfw", -- Links glfw.lib
		"glad", 
		"imgui",
		"spdlog",
		"pfd",
		"stb_image",
		"cxxopts",
		"cryptopp",
		"inih", 
		"json",
		"sqlitecpp",
		"curl", -- Order matter, Link curl before zlib
		"cpr",
		--"zlib",
		"tabulate",
		"infoware",
	}


	---[[ Platform ]]---
	filter "system:windows"
		systemversion "latest"
		--entrypoint "WinMainCRTStartup" -- use WinMain as entry point

		-- .exe icon
	    files 
	    {
			'Enigma.rc', '**.ico' 
	 	}
	    vpaths 
	    {
	    	--[[
				uncomment if files{ "Resources/**.*" } is deleted above
	    	--]]
	  	 	--['Resources/*'] = { '*.rc', '**.ico' } 
	    }
		--

		defines
		{
			"ENIGMA_PLATFORM_WINDOWS",

			-- Common MSVC warnings defines
			"_CRT_SECURE_NO_DEPRECATE",
			"_CRT_SECURE_NO_WARNINGS",
			"_CRT_NONSTDC_NO_WARNINGS",
			"_SILENCE_ALL_CXX17_DEPRECATION_WARNINGS"
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
			-- static libraries do not include their dependencies => https://stackoverflow.com/questions/58118344/transitive-library-dependency-in-premake
			"zlib",
			"mbedtls",

			"GL", -- link opengl lib
			"X11", -- x11 Linux gui libs
			"dl", -- dynamic loader interface
			"pthread", -- std::thread
			"stdc++fs", -- std::filesystem
			"tbb" -- std::execution
		}

	filter "system:macosx"
		defines
		{
			"ENIGMA_PLATFORM_MACOS",
		}	
		links
		{
			-- TODO
			"GL", -- link opengl lib
			"pthread", -- std::thread
			"stdc++fs" -- std::filesystem
		}

	
	---[[ Configurations ]]---
	filter "configurations:Debug"
		kind "ConsoleApp" -- Debug we need console for logging
		defines "ENIGMA_DEBUG"
		runtime "Debug"
		symbols "Full" --vs17 and newer | symbols "On"
		optimize "Off" -- No optimization will be performed.
				
		includedirs
		{
			"%{IncludeDir.catch2}", -- we use catch2 tests only in debug
		}
		links
		{
			"catch2", -- we use catch2 tests only in debug
		}


	filter "configurations:Release"
		kind "ConsoleApp" -- we need console for logging in CLI
		--kind "WindowedApp" -- Release as windowed application
		defines "ENIGMA_RELEASE"
		runtime "Release"
		optimize "On" -- Perform a balanced set of optimizations.
		inlining "Explicit" -- Only inline functions explicitly marked with the inline keyword.
		
		flags
		{
			-- Only applicable in Release configurations
			"LinkTimeOptimization", -- Link Time Optimization (LTO) refers to program optimization during linking. The linker pulls all object files together and combines them into one program. The linker can see the whole of the program, and can therefore do whole-program analysis and optimization.
		}

	filter "configurations:Dist"
		kind "ConsoleApp" -- we need console for logging in CLI
		--kind "WindowedApp" -- Distribution as windowed application
		defines "ENIGMA_DIST"
		runtime "Release"
		optimize "Full" -- Full optimization.
		inlining "Auto" -- Inline any suitable function for full performance
		
		flags
		{
			-- Only applicable in Release configurations
			"LinkTimeOptimization", -- Link Time Optimization (LTO) refers to program optimization during linking. The linker pulls all object files together and combines them into one program. The linker can see the whole of the program, and can therefore do whole-program analysis and optimization.
		}
