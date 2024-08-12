workspace "tools"
	architecture "x64"

    configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["da-core"] = "%{wks.location}/../src/DAEngine"
IncludeDir["glm"] = "%{wks.location}../thirdparty/glm"

group "Tools"

include "shaderbgfx"
include "assetbuilder"

group ""