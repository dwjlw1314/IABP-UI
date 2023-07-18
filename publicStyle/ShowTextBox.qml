import QtQuick 2.0

Item {
    id: mainWindow
    property string text: "II"

    width: 70
    height: 30

    Rectangle {
        color: "#5A5568"
        radius: 10
        border.width: 1

        anchors.fill: parent

        Text {
            text: mainWindow.text
            anchors.fill: parent
            color: "white"
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 16
        }
    }
}
