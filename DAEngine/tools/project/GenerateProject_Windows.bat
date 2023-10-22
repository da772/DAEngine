%~d0
cd %~dp0
cd ..
call ..\..\thirdparty\premake\premake5.exe vs2022 --os=windows
PAUSE