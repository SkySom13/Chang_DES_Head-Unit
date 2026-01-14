import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Design 1.0

Window {
    visible: true
    // IVI Shell Surface ID: 0xBEEF0001 (Battery Meter)
    property var iviSurfaceId: 0xBEEF0001
    
    // Fullscreen Weston Display (1024x600)
    x: 0
    y: 0
    width: 1024
    height: 600
    flags: Qt.FramelessWindowHint
    title: "Battery Meter"
    color: Constants.backgroundColor
    opacity: 1.0

    Rectangle {
        anchors.fill: parent
        color: Constants.backgroundColor

        Column {
            anchors.centerIn: parent
            spacing: 20

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "BATTERY LEVEL"
                font.pixelSize: Constants.largeFontSize
                font.bold: true
                color: Constants.accentColor
            }

            Rectangle {
                width: 120
                height: 200
                anchors.horizontalCenter: parent.horizontalCenter
                color: Constants.primaryColor
                border.color: Constants.accentColor
                border.width: 2
                radius: 10

                Rectangle {
                    anchors {
                        left: parent.left
                        bottom: parent.bottom
                        margins: 4
                    }
                    width: parent.width - 8
                    height: (parent.height - 8) * 0.75
                    color: Constants.accentColor
                    radius: 8
                }

                Text {
                    anchors.centerIn: parent
                    text: "75%"
                    font.pixelSize: Constants.largeFontSize
                    font.bold: true
                    color: Constants.accentColor
                }
            }

            Text {
                anchors.horizontalCenter: parent.horizontalCenter
                text: "Status: Connected"
                font.pixelSize: Constants.fontSize
                color: Constants.textColor
            }
        }
    }
}
