workspace "Enigma"  -- Workspace which is the solution in windows, in xcode will be something else
	architecture "x86_64" -- Targeted architecture x64
	startproject "Enigma Application" -- Set 'Enigma Application' project as start up project

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


-- Organize libs solution in a single filtered directory 
group "Dependencies"
	include "Dependencies/glfw" --there is a premake5.lua in Dependencies/glfw/ which will be detected and set things up for us
	include "Dependencies/glad"
	include "Dependencies/imgui"
	include "Dependencies/spdlog"
	include "Dependencies/pfd"
	include "Dependencies/stb_image"
group ""

-- Include Enigma Application projects solution
include "Enigma"
include "Enigma Application"