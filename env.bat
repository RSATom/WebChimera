REM MSBUILD PATH   reg.exe query "HKLM\SOFTWARE\Microsoft\MSBuild\ToolsVersions\4.0" /v MSBuildToolsPath

set PATH=%PATH%;C:\VLC
set PATH=%PATH%;C:\Python27
set PATH=%PATH%;C:\Program Files/ (x86)\CMake\bin
set PATH=%PATH%;C:\Windows\Microsoft.NET\Framework64\v4.0.30319


set QTDIR=c:\Qt-5.5.1-static-MT

set CMAKE_DIR=%QTDIR%\lib\cmake

set Qt5Core_DIR=%CMAKE_DIR%\Qt5Core
set Qt5Qml_DIR=%CMAKE_DIR%\Qt5Qml
set Qt5Quick_DIR=%CMAKE_DIR%\Qt5Quick
set Qt5_DIR=%CMAKE_DIR%\Qt5


"C:\Program Files (x86)\Microsoft Visual Studio 12.0\Common7\Tools\vsvars32.bat"



