@echo off
"%WIX%\bin\heat" dir %QTDIR%\qml\QtQuick\Controls -gg -sfrag -t qt_quick_controls.xslt -o qt_quick_controls.wxs
if ERRORLEVEL 1 pause
