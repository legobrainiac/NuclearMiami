@echo off

cd C:\Program Files (x86)\Microsoft Visual Studio\2017\Community\MSBuild\15.0\Bin\

MSBuild.exe C:\Repositories\1DAE_PROG_FINAL\Pinto_Tomas_NuclearMiami\Pinto_Tomas_NuclearMiami.sln /t:Build /p:Configuration=Release /property:Platform=x86

set BUILD_STATUS=%ERRORLEVEL%

cd C:\Repositories\1DAE_PROG_FINAL\Pinto_Tomas_NuclearMiami\Release

if %BUILD_STATUS%==0  (
	Pinto_Tomas_NuclearMiami.exe
)
