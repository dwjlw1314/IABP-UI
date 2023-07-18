import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Item {
    id: mainWindow

    property var progressBarRadius: [12,0,0,12]
    property real progressBarValue: 0.05

    function setProgressBarRadius(value) {
        progressBarValue = value
        if (value >= 0.99)
            progressBarRadius = [12,10,10,12]
        else if (value >= 0.96)
            progressBarRadius = [12,6,6,12]
        else if (value >= 0.94)
            progressBarRadius = [12,2,2,12]
        else
            progressBarRadius = [12,1,1,12]
    }

//    Timer {
//        interval: 2000
//        running: true
//        repeat: true
//        onTriggered: {
//            setProgressBarRadius(Math.random())
//        }
//    }

    Connections {
        target: publicData
        onHeliumValueChange : {
            setProgressBarRadius(he_value)
        }
    }

    RoundRectangle
    {
        id: up
        width: mainWindow.width - 25
        height: 6
        radius: 2

        border.width: 2
        border.color: "white"
        x: (mainWindow.width - width) / 2
    }

    Canvas {
        id: canvasMain

        width: mainWindow.width
        height: 10
        contextType: "2d"
        anchors.left: mainWindow.left
        y: up.y

        onPaint: {
            context.Antialiasing = true
            context.lineWidth = 2
            context.lineJoin = "bevel"
            //context.lineCap  = "square"
            context.strokeStyle = "white"
            context.beginPath();
            context.moveTo(up.x + up.width/2 - 3, up.y + up.height)
            context.lineTo(up.x + up.width/2 - 3, up.y + up.height + 5);
            context.moveTo(up.x + up.width/2 + 3, up.y + up.height)
            context.lineTo(up.x + up.width/2 + 3, up.y + up.height + 5);
            context.closePath();
            context.stroke();
        }
    }

    RoundRectangle
    {
        id: down
        width: mainWindow.width - 10
        anchors.bottom: textInfo.top
        anchors.top: canvasMain.bottom
        radius: 50

        border.width: 3
        border.color: "white"
        x: (mainWindow.width - width) / 2

    }

    ProgressBar {
        id: progressBar
        anchors.fill: down
        anchors.margins: 4
        orientation: Qt.Vertical
        value: mainWindow.progressBarValue
        //implicitWidth: 10
        style: progressBarStyle
    }

    Component {
        id: progressBarStyle
        ProgressBarStyle {
            background: Rectangle {
                color: "transparent"
                border.color: "transparent"
            }
            progress: ShapeCustom {
                color: "white"
                //border.color: "red"
                anchors.fill: parent
                //anchors.margins: 1
                //radius: 50
                borderWidth: 0
                cornersRadius: {
                    mainWindow.progressBarRadius
                }
            }


        }
    }

//    RoundRectangle
//    {
//        id: innerlayer
//        anchors.bottom: textInfo.top
//        anchors.bottomMargin: 3
//        anchors.top: canvasMain.bottom
//        anchors.topMargin: 3
//        anchors.left: down.left
//        anchors.leftMargin: 3
//        anchors.right: down.right
//        anchors.rightMargin: 3
//        radius: 50
//        color: "white"
//        border.width: 1
//        border.color: "black"
//    }

    Text {
        id: textInfo
        text: qsTr("He")
        x: (mainWindow.width - width) / 2
        anchors.bottom: mainWindow.bottom
        anchors.bottomMargin: 0
        horizontalAlignment: Text.AlignHCenter
        color:"white"
        font.bold: true
        font.pointSize: 12
    }
}
