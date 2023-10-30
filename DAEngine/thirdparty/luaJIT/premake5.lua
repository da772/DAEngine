-- Used in BuildVM


function getrootpath()
    return "%{prj.location}../.."
end

function buildpath(p) 
    return "%{getrootpath()}/bin/%{cfg.shortname}/" .. (p or "")
end

function srcpath(p)
    return "%{getrootpath()}/src/" .. (p or "")
end

    include "src/host/buildvm.premake5.lua"
    include "src/host/minilua.premake5.lua"
    include "luajit.premake5.lua"

