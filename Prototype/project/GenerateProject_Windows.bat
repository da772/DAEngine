%~d0
cd %~dp0
cd ..
xcopy "BuildTarget.lua" "../premake5.lua*" /K /D /H /Y /I
cd ..
call ThirdParty\premake\premake5.exe vs2019 --os=windows
PAUSE