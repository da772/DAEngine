workspace "tools"
	architecture "x64"

    configurations
	{
		"Debug",
		"Release"
	}

outputdir = "%{cfg.buildcfg}-%{cfg.system}-%{cfg.architecture}"

IncludeDir = {}
IncludeDir["daengine-core"] = "%{wks.location}/../src/DAEngine"
IncludeDir["glm"] = "%{wks.location}/../thirdparty/glm/include"

group "Tools"

include "shaderbgfx"
include "assetbuilder"

group ""