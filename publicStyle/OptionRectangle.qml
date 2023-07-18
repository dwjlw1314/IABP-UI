import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    id: mainWindow
    property int topBtnWidth: 180
    property int topBtnHeight: 45
    property string btnName: ""
    property int pointSize: 18
    property string textColor: "white"
    property bool popStatus: true
    property int popAreaH: 520

    radius: 12
    width: topBtnWidth
    height: topBtnHeight

    color: "#EDF2FF"
//    gradient: Gradient {
//        GradientStop { position: 0.0; color: "lightsteelblue" }
//        //GradientStop { position: 0.5; color: "lightslategraY" }
//        GradientStop { position: 1.0; color: "dodgerblue" }
//    }

    signal showStatusChange(var objectid, var p_opacity, var p_height)

    function setMouseEvent(flag)
    {
        if (popStatus !== flag)
            return
        triggerClicked()
    }

    function triggerClicked()
    {
        if (popStatus)
        {
            btn.color = "white"
            textColor = "black"
            mainWindow.width = mainWindow.topBtnWidth + 10
            mainWindow.anchors.leftMargin = mainWindow.anchors.leftMargin - 5
            btn.tmpH = popAreaH
            btn.tmpO = 1
        }
        else
        {
            btn.color = "#99AFEE"
            textColor = "white"
            mainWindow.width = mainWindow.topBtnWidth
            mainWindow.anchors.leftMargin = mainWindow.anchors.leftMargin + 5
            btn.tmpH = topBtnHeight
            btn.tmpO = 0
        }

        mainWindow.height = btn.tmpH
        mainWindow.showStatusChange(mainWindow, btn.tmpO, btn.tmpH-topBtnHeight)
        popStatus = !popStatus
    }

    Rectangle {
        id: btn
        width: parent.width
        height: topBtnHeight
        radius: parent.radius
        border.width: 2
        border.color: "white"
        color: "#99AFEE"
        anchors.left: parent.left
        anchors.top: parent.top
        Text {
            text: qsTr(btnName)
            anchors.centerIn: parent
            color: textColor

            font.pointSize: pointSize
        }

        property int tmpH: 0
        property int tmpO: 0

        MouseArea {
            anchors.fill: parent;
            hoverEnabled: false;
            onClicked:
            {
                triggerClicked()
            }
            onPressed: {
                btn.color = "darkorange"
                textColor = "black"
                console.log(btnName + " button pressed")
            }
            onReleased: {
                 console.log(btnName + " button released")
            }
        }
    }
}


