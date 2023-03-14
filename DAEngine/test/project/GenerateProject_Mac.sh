#!/bin/bash
here="`dirname \"$0\"`"
cd "$here"
cd ..
../../ThirdParty/premake5_mac gmake2 --unit-test
