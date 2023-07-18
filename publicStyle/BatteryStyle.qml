import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    id: mainWindow

    property real power1: 0.7
    property real power2: 0.7

    Rectangle {
        id: top1
        border.width: 3
        border.color: "white"
        x: left.width/2 - width/2
        y: left.y - 6
        width: mainWindow.width/6
        height: 10
        radius: 4
        color: "transparent"
    }

    RoundRectangle
    {
        id: left
        width: mainWindow.width/3
        anchors.bottom: textInfo.top
        anchors.top: mainWindow.top
        anchors.topMargin: 8
        anchors.left: mainWindow.left
        radius: 6
        color: "black"
        border.width: 3
        border.color: "white"
    }

    ProgressBar {
        id: progressBarLeft
        anchors.fill: left
        anchors.margins: 4
        orientation: Qt.Vertical
        value: mainWindow.power1
        //implicitWidth: 10
        style: progressBarStyle
    }

    Text {
        id: textInfo
        text: qsTr("1")
        x: top1.x + 4
        anchors.bottom: mainWindow.bottom
        anchors.bottomMargin: 0
        color:"white"
        font.bold: true
        font.pointSize: 14
    }

    Rectangle {
        id: top2
        border.width: 3
        border.color: "white"
        x: mainWindow.width - right.width + (right.width - width)/2
        y: left.y - 6
        width: mainWindow.width/6
        height: 10
        radius: 4
        color: "transparent"
    }

    RoundRectangle
    {
        id: right
        width: mainWindow.width/3
        anchors.bottom: textInfo2.top
        anchors.top: mainWindow.top
        anchors.topMargin: 8
        anchors.right: mainWindow.right
        radius: 6
        color: "black"
        border.width: 3
        border.color: "white"
    }

    ProgressBar {
        id: progressBarRight
        anchors.fill: right
        anchors.margins: 4
        orientation: Qt.Vertical
        value: mainWindow.power2
        //implicitWidth: 10
        style: progressBarStyle
    }

    Text {
        id: textInfo2
        text: qsTr("2")
        x: top2.x + 4
        anchors.bottom: mainWindow.bottom
        anchors.bottomMargin: 0
        color:"white"
        font.bold: true
        font.pointSize: 14
    }

    Component {
        id: progressBarStyle
        ProgressBarStyle {
            background: Rectangle {
                color: "transparent"
                border.color: "transparent"
            }
            progress: RoundRectangle {
                anchors.fill: parent
                radius: 2
                color: "white"
                border.width: 0
                border.color: "white"
            }
        }
    }
}
