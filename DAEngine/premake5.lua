newoption {
	trigger = "unit-test",
	description = "builds for unit tests"
}

if not _OPTIONS['unit-test'] then
IncludeDir = {}
IncludeDir["GLFW"] = "%{wks.location}/DAEngine/thirdparty/GLFW/include"
IncludeDir["Vulkan"] = "%{wks.location}/DAEngine/thirdparty/Vulkan"
IncludeDir["ImGui"] = "%{wks.location}/DAEngine/thirdparty/DearImGui"
IncludeDir["glm"] = "%{wks.location}/DAEngine/thirdparty/glm"
IncludeDir["stb"] = "%{wks.location}/DAEngine/thirdparty/stb"
IncludeDir["assimp"] = "%{wks.location}/DAEngine/thirdparty/assimp/include"
IncludeDir["zlib"] = "%{wks.location}/DAEngine/thirdparty/zlib/include"
IncludeDir["ktx"] = "%{wks.location}/DAEngine/thirdparty/ktx/include"
IncludeDir["bx"] = "%{wks.location}/DAEngine/thirdparty/bx/include"
IncludeDir["bimg"] = "%{wks.location}/DAEngine/thirdparty/bimg/include"
IncludeDir["bgfx"] = "%{wks.location}/DAEngine/thirdparty/bgfx/"
IncludeDir["luajit"] = "%{wks.location}/DAEngine/thirdparty/luajit/include"
IncludeDir["lua51"] = "%{wks.location}/DAEngine/thirdparty/lua51/include"
IncludeDir["bullet3"] = "%{wks.location}/DAEngine/thirdparty/bullet3/include"
IncludeDir["sol2"] = "%{wks.location}/DAEngine/thirdparty/sol2/include"
IncludeDir["enet"] = "%{wks.location}/DAEngine/thirdparty/enet/include"
IncludeDir["miniupnpc"] = "%{wks.location}/DAEngine/thirdparty/miniupnpc/include"
IncludeDir["recast"] = "%{wks.location}/DAEngine/thirdparty/recast/Recast/include"
IncludeDir["recast-debugutils"] = "%{wks.location}/DAEngine/thirdparty/recast/DebugUtils/include"
IncludeDir["recast-detour"] = "%{wks.location}/DAEngine/thirdparty/recast/Detour/include"
IncludeDir["recast-detour-crowd"] = "%{wks.location}/DAEngine/thirdparty/recast/DetourCrowd/include"
IncludeDir["recast-detour-tilecache"] = "%{wks.location}/DAEngine/thirdparty/recast/DetourTileCache/include"
IncludeDir["optick"] = "%{wks.location}/DAEngine/thirdparty/optick/include"

IncludeDir["da-core"] = "%{wks.location}/DAEngine/src/DAEngine/"


group "daengine"
include "src/DAEngine/core"
include "src/DAEngine/stl"
include "src/DAEngine/graphics"
include "src/DAEngine/debug"
include "src/DAEngine/net"
include "src/DAEngine/physics"
include "src/DAEngine/script"
include "src/DAEngine/ai"
include "src/DAEngine/platform"
group ""

group "3rdParty"
if os.host() == "macosx" then
	include "thirdparty/GLFW"
	include "thirdparty/DearImGui"
	include "thirdparty/zlib"
	include "thirdparty/assimp"
	include "thirdparty/ktx"
	include "thirdparty/bx"
	include "thirdparty/bimg"
	include "thirdparty/bgfx"
	include "thirdparty/lua51"
	include "thirdparty/bullet3"
	include "thirdparty/enet"
	include "thirdparty/miniupnpc"
	include "thirdparty/recast"
	include "thirdparty/optick"
end

if os.host() == "windows" then
	include "thirdparty/GLFW"
	include "thirdparty/DearImGui"
	include "thirdparty/zlib"
	include "thirdparty/assimp"
	include "thirdparty/ktx"
	include "thirdparty/bx"
	include "thirdparty/bimg"
	include "thirdparty/bgfx"
	include "thirdparty/luajit"
	include "thirdparty/bullet3"
	include "thirdparty/enet"
	include "thirdparty/miniupnpc"
	include "thirdparty/recast"
	include "thirdparty/optick"
end

if os.host() == "linux" then
	include "thirdparty/GLFW"
	include "thirdparty/DearImGui"
	include "thirdparty/zlib"
	include "thirdparty/assimp"
	include "thirdparty/ktx"
	include "thirdparty/bx"
	include "thirdparty/bimg"
	include "thirdparty/bgfx"
	include "thirdparty/lua51"
	include "thirdparty/bullet3"
	include "thirdparty/enet"
	include "thirdparty/miniupnpc"
	include "thirdparty/recast"
	include "thirdparty/optick"
end

group ""
end