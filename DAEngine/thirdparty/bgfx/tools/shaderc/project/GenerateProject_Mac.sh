#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
cp -f "BuildTarget.lua" "../premake5.lua"
cd ..
../premake5_mac gmake2 --os=macosx