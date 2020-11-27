project "catch2"
	kind "StaticLib"
    language "C++"
	staticruntime "on"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

    includedirs 
    {
     	"src/", 
    }

    files 
    {
      "src/catch2/**.cpp",
      "src/catch2/**.hpp",
    }
    
    
	--- Platform ---
	filter "system:windows"
		systemversion "latest"
		defines
		{
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
		}

	filter "system:macosx"
		defines
		{
		}	
		links
		{
		}


	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off"

	filter "configurations:Release"
		runtime "Release"
		optimize "On"

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full"