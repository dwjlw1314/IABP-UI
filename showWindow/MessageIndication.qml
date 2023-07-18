import QtQuick 2.0
import QtCharts 2.3
import "../publicStyle"

Item {
    id: mainWindow

    property int poltWidth : 990

    ShapeRectangle {
        id: leftArea
        width: mainWindow.poltWidth
        drawLine: true
    }

    AlarmInformation {
        id: alarmInfo

        anchors.top: mainWindow.top
        anchors.topMargin: 10
        anchors.bottom: mainWindow.bottom
        anchors.bottomMargin: 5
        anchors.left: mainWindow.left
        anchors.leftMargin: 5
        anchors.right: leftArea.right
    }

    ShapeRectangle {
        id: rightArea
        width: parent.width - leftArea.width
        anchors.left: leftArea.right
        anchors.leftMargin: 3
    }

//    Component.onCompleted: {
//        console.log("helium-> " + mainWindow.width + "--" + mainWindow.height)
//    }

    BatteryStyle {
        id: batteryArea
        width: 110
        height: mainWindow.height - 10

        anchors.top: rightArea.bottom
        anchors.topMargin: 10-rightArea.height
        anchors.right: heliumArea.left
        anchors.rightMargin: 20
    }

    HeliumCylinderStyle {
        id: heliumArea

        width: 40
        height: mainWindow.height - 25

        anchors.top: rightArea.bottom
        anchors.topMargin: 15-rightArea.height
        anchors.right: mainWindow.right
        anchors.rightMargin: 15
    }
}
