#!/bin/sh

./configure -prefix $HOME/Qt-5.4.0-static -opensource -confirm-license -static -debug-and-release -no-framework -no-sql-sqlite -no-qml-debug -nomake examples -nomake tests -skip qtactiveqt -skip qtenginio -skip qtlocation -skip qtmultimedia -skip qtserialport -skip qtquick1 -skip qtscript -skip qtsensors -skip qtwebengine -skip qtwebkit -skip qtwebkit-examples -skip qtwebsockets -skip qtxmlpatterns -skip qtlocation -no-widgets
