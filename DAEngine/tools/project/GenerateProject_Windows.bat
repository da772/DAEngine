%~d0
cd %~dp0
cd ..
call ..\..\thirdparty\premake\premake5.exe vs2019 --os=windows
PAUSE