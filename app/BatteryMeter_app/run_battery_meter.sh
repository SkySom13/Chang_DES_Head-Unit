#!/bin/bash
export DISPLAY=:0
export QT_QPA_PLATFORM=xcb
export LD_LIBRARY_PATH=~/Qt/5.12.2/gcc_64/lib:$LD_LIBRARY_PATH
export QTDIR=~/Qt/5.12.2/gcc_64
export PATH=~/Qt/5.12.2/gcc_64/bin:$PATH
export QT_PLUGIN_PATH=~/Qt/5.12.2/gcc_64/plugins:$QT_PLUGIN_PATH
export QML2_IMPORT_PATH=~/Qt/5.12.2/gcc_64/qml:$QML2_IMPORT_PATH
./build/BatteryMeter_app
