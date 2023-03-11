workspace "tools"
	architecture "x64"

    configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"
group "Tools"

include "shaderbgfx"

group ""