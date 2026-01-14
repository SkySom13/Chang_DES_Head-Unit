import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import Design 1.0

Window {
    visible: true
    // IVI Shell Surface ID: 0xBEEF0003 (Gear State)
    property var iviSurfaceId: 0xBEEF0003
    
    // Fullscreen Weston Display (1024x600)
    x: 0
    y: 0
    width: 1024
    height: 600
    title: "Gear State"
    flags: Qt.FramelessWindowHint
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
                text: "GEAR STATE"
                font.pixelSize: Constants.largeFontSize
                font.bold: true
                color: Constants.accentColor
            }

            Rectangle {
                width: 200
                height: 200
                anchors.horizontalCenter: parent.horizontalCenter
                color: Constants.primaryColor
                border.color: Constants.accentColor
                border.width: 2
                radius: 10

                Text {
                    anchors.centerIn: parent
                    text: "P"
                    font.pixelSize: 80
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
