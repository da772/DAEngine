%~d0
cd %~dp0
call ..\..\..\..\DAEngine\tools\assetbuilder\bin\Release-windows-x86_64\assetbuilder\assetbuilder.exe %~dp0..\ %~dp0..\..\..\src\game\generated\ --debug
PAUSE