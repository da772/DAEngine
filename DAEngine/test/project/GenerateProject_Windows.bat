%~d0
cd %~dp0
cd ..
call ..\..\ThirdParty\premake\premake5.exe vs2022 --os=windows --unit-test
PAUSE