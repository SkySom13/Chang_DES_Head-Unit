import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15

Window {
    visible: true
    width: 280
    height: 400
    flags: Qt.FramelessWindowHint
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

    // ===== BATTERY SECTION (From IC_app) =====
    Rectangle {
        id: batterySection
        anchors.fill: parent
        color: "transparent"

        // Background gauge outer ring
        Image {
            id: gaugeSpeedometer_Ticks4_battery
            x: 0
            y: 60
            width: 280
            height: 280
            source: "qrc:/images/GaugeSpeedometer_Ticks2.png"
            fillMode: Image.PreserveAspectFit
        }

        // Inner gauge ring with "Battery" label
        Image {
            id: gaugeSpeedometer_Ticks5_battery
            anchors.centerIn: gaugeSpeedometer_Ticks4_battery
            source: "qrc:/images/GaugeSpeedometer_Ticks1.png"
            fillMode: Image.PreserveAspectFit

            TextInput {
                x: 134
                y: 265
                width: 195
                height: 49
                color: "#730000"
                text: qsTr("Battery")
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                anchors.horizontalCenter: parent.horizontalCenter
                readOnly: true
            }
        }

        // Dynamic battery fill with color coding
        Rectangle {
            id: battery_fill
            width: 68
            height: 110 * batteryLevel / 100
            x: 103
            y: 258 - height
            z: 1
            color: batteryLevel <= 20 ? "#ff4444"
                 : batteryLevel <= 60 ? "#ffaa33"
                 : "#57e389"
        }

        // Battery outline icon
        Image {
            id: battery_outline_icon
            x: 80
            y: 80
            width: 120
            source: "qrc:/images/battery_outline_icon.png"
            fillMode: Image.PreserveAspectFit
            z: 2
        }

        // Battery percentage text
        Text {
            id: battery_text
            anchors.centerIn: battery_outline_icon
            font.pixelSize: 25
            font.bold: true
            color: "white"
            text: batteryLevel + "%"
            z: 3
        }

        // Bolt icon (charging indicator)
        Image {
            id: bolt_icon
            anchors.centerIn: battery_outline_icon
            anchors.verticalCenterOffset: -50
            width: 40
            height: 40
            source: "qrc:/images/bolt_icon.png"
            fillMode: Image.PreserveAspectFit
            z: 4
            visible: false  // TODO: Add isCharging property
        }

        // Service status indicator
        Rectangle {
            x: 10
            y: 10
            width: 15
            height: 15
            radius: 7.5
            color: serviceAvailable ? "#57e389" : "#ff4444"
            border.color: "#ffffff"
            border.width: 2
        }
    }
}
