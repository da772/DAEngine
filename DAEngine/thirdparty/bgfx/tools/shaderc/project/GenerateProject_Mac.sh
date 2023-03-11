#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
../../../../../thirdparty/premake5_mac gmake2 --os=macosx
