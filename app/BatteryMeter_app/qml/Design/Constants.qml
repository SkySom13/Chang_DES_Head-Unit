pragma Singleton
import QtQuick 2.15

QtObject {
    // Battery section dimensions from IC_app
    readonly property int width: 280
    readonly property int height: 400
    
    // Colors matching IC_app design
    readonly property color backgroundColor: "#000000"
    readonly property color labelColor: "#730000"
    readonly property color textColor: "#ffffff"
    
    // Battery color states
    readonly property color batteryLow: "#ff4444"      // Red: ≤20%
    readonly property color batteryMedium: "#ffaa33"   // Orange: 21-60%
    readonly property color batteryHigh: "#57e389"     // Green: 61-100%
    
    // Font sizes
    readonly property int labelFontSize: 20
    readonly property int valueFontSize: 25
}