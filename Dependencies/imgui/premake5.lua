project "imgui"
	kind "StaticLib"
	language "C++"
	cppdialect "C++17"
	staticruntime "On"

	targetdir ("Bin/" .. outputdir .. "/%{prj.name}")
	objdir ("Bin-Intermediate/" .. outputdir .. "/%{prj.name}")

    --warnings "Off" -- turn off warnings for this library

	files
	{
		"imconfig.h",
		"imgui.h",
		"imgui.cpp",
		"imgui_draw.cpp",
		"imgui_internal.h",
		"imgui_widgets.cpp",
		"imstb_rectpack.h",
		"imstb_textedit.h",
		"imstb_truetype.h",
		"imgui_demo.cpp"
	}

	-- Platform --
	--[[ Windows --]]
	filter "system:windows"
		systemversion "latest"
	

	--- Configurations ---
	filter "configurations:Debug"
		runtime "Debug"
		symbols "On"
		optimize "Off" -- No optimization will be performed.

	filter "configurations:Release"
		runtime "Release"
		optimize "On" -- Perform a balanced set of optimizations.
		inlining "Explicit" -- Only inline functions explicitly marked with the inline keyword.

	filter "configurations:Dist"
		runtime "Release"
		optimize "Full" -- Full optimization.
		inlining "Auto" -- Inline any suitable function for full performance