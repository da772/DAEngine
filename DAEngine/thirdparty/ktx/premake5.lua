project "ktx"
    kind "StaticLib"
    language "C"
    staticruntime "on"
			
	targetdir ("bin/" .. outputdir .. "/%{prj.name}")
    objdir ("bin-obj/" .. outputdir .. "/%{prj.name}")

	files
	{
        "include/**.h",     
        "lib/**.c",
        "lib/**.h"
    }
	
	includedirs
	{
		"include",
		"other_include",
		"lib",
		"%{IncludeDir.Vulkan}/include"
	}
	

   