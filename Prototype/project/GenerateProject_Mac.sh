#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
cp -f "BuildTarget.lua" "../premake5.lua"
cd ..
./ThirdParty/premake5_mac xcode4 --os=macosx