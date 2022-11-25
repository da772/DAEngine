project "zlib"
	language    "C"
	kind        "StaticLib"
	staticruntime "on"
	defines     { "N_FSEEKO" }
	warnings    "off"

	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")
	
	files
	{
		"source/**.h",
		"source/**.c"
	}

	filter "system:windows"
		systemversion "latest"
		defines { "_WINDOWS" }

	filter "system:not windows"
		defines { 'HAVE_UNISTD_H' }
