@echo off
for %%i in ("%~0") do set THIS_BAT_PATH=%%~dpi
set SAVE_CD=%CD%
cd %THIS_BAT_PATH%\..\..\
call prep2008.cmd "projects\Chimera" "build_Chimera_vs2008"
cd %SAVE_CD%
pause
