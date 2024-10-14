%~d0
cd %~dp0
cd ..
call ..\..\..\DAEngine\tools\assetbuilder\bin\Release-windows-x86_64\assetbuilder\assetbuilder.exe %cd% %~dp0..\..\..\src\game\generated\ --debug
PAUSE