import QtQuick 2.12
import QtQuick.Window 2.12
import QtQuick.Controls 2.12

Window {
    visible: true
    width: 280
    height: 400
    // flags: Qt.FramelessWindowHint
    title: "Battery Meter"
    color: "#000000"

    // Property bindings (will receive from VehicleControlECU via vsomeip)
    property int batteryLevel: vehicleClient.batteryLevel
    property bool serviceAvailable: vehicleClient.serviceAvailable

    Component.onCompleted: {
        console.log("🔋 BatteryMeter initialized")
        console.log("   Service available:", serviceAvailable)
        console.log("   Battery level:", batteryLevel, "%")
    }

    Connections {
        target: vehicleClient
        function onBatteryLevelChanged() {
            batteryLevel = vehicleClient.batteryLevel
            console.log("📡 Battery changed:", batteryLevel, "%")
        }
        function onServiceAvailableChanged() {
            serviceAvailable = vehicleClient.serviceAvailable
            console.log("🔗 Service availability:", serviceAvailable)
        }
    }

    // ===== BATTERY SECTION (Simplified for debugging) =====
    Rectangle {
        id: batterySection
        anchors.fill: parent
        color: "#000000"

        // Title
        Text {
            id: titleText
            x: 10
            y: 10
            text: "Battery Meter"
            color: "#ffffff"
            font.pixelSize: 20
            font.bold: true
        }

        // Service status indicator
        Rectangle {
            x: 10
            y: 40
            width: 15
            height: 15
            radius: 7.5
            color: serviceAvailable ? "#57e389" : "#ff4444"
            border.color: "#ffffff"
            border.width: 2
        }

        // Simple battery visualization
        Rectangle {
            id: batteryContainer
            x: 50
            y: 100
            width: 180
            height: 200
            color: "#333333"
            border.color: "#ffffff"
            border.width: 2

            // Battery fill
            Rectangle {
                id: battery_fill
                anchors.bottom: parent.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                height: parent.height * batteryLevel / 100
                color: batteryLevel <= 20 ? "#ff4444"
                     : batteryLevel <= 60 ? "#ffaa33"
                     : "#57e389"
            }

            // Battery percentage text
            Text {
                anchors.centerIn: parent
                text: batteryLevel + "%"
                color: "white"
                font.pixelSize: 25
                font.bold: true
            }
        }

        // Gear display
        Text {
            x: 10
            y: 320
            text: "Gear: " + vehicleClient.gearState
            color: "#ffffff"
            font.pixelSize: 16
        }

        // Speed display
        Text {
            x: 10
            y: 350
            text: "Speed: " + vehicleClient.speed + " km/h"
            color: "#ffffff"
            font.pixelSize: 16
        }
    }
}
