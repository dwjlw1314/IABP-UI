import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id: mainWindow
    anchors.left: parent.left
    anchors.leftMargin: marginH/2
    antialiasing: true

    property string text: ""
    property int pointSize: 9
    property string orgColor: "#D1D3F8"
    property string curColor: "#D1D3F8"
    property string textColor: "black"
    property string orgBorderColor: "#C2C4EB"
    property string borderColor: "#C2C4EB"
    property int marginH: 40
    property int btnID: 0
    property real textMargin: 0.7

    property bool selected: false
    property int btnRadius: 12

    signal btnSelectStatus(var id, var flag)

    onCurColorChanged: {
        button.color = curColor
    }

    Rectangle {
        id: button
        width: mainWindow.width - marginH
        height: mainWindow.height
        color: mainWindow.curColor
        border.width: 2
        border.color: mainWindow.borderColor
        radius: mainWindow.btnRadius

        Text {
            text: mainWindow.text
            clip: true
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.fill: parent
            font.pointSize: pointSize
            lineHeight: 0.7
            color: mainWindow.textColor
        }

        MouseArea {
            anchors.fill: parent
            hoverEnabled: false;
            onClicked: {
                mainWindow.curColor = "white"
                mainWindow.btnSelectStatus(mainWindow, btnID)
            }

            onPressed: {
                button.color = "darkorange"
            }
        }
    }
}
