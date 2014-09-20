@echo off
for %%i in ("%~0") do set THIS_BAT_PATH=%%~dpi
set SAVE_CD=%CD%
cd %THIS_BAT_PATH%\deps\firebreath\
call prep2013.cmd "..\..\src" "..\..\build_win_vc2013" -Wno-dev
cd %SAVE_CD%
pause
