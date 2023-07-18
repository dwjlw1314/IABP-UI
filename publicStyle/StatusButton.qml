import QtQuick 2.0

Item {
    id: mainWindow
    property var objectid
    property string btnText: ""
    property string downText: ""
    property int pointSize: 13
    property int popAreaH: 50
    property bool status: false
    property bool enablePopArea: false  //是否启用弹窗
    property bool enablePressed: false //是否启用按压计时
    property alias downTextVisible: downTextArea.visible

    readonly property int btnAreaH: 50

    signal showStatusChange(var objectid, var p_opacity, var p_height, var status)

    function hideArea()
    {
        if (mainWindow.status && enablePopArea)
        {
            selectBtn.gradient = gradientBtn
            textArea.color = "white"
            selectBtn.border.color = "#6C6F85"
            backgroundArea.visible = false
            mainWindow.width = mainWindow.width - 10
            mainWindow.anchors.leftMargin = mainWindow.anchors.leftMargin + 5
            mainWindow.status = false
        }
    }

    Rectangle {
        id: backgroundArea

        radius: 12
        visible: false
        color: "#EDF2FF"
        width: mainWindow.width
        height: mainWindow.btnAreaH + mainWindow.popAreaH
        anchors.bottom: selectBtn.bottom
    }

    Rectangle {
        id: selectBtn

        gradient: Gradient {
            id : gradientBtn
            GradientStop { position: 0.0; color: "#474B6A" }
            //GradientStop { position: 0.5; color: "lightslategraY" }
            GradientStop { position: 1.0; color: "#777CBC" }
        }

        property var orgGradient: Gradient {
            id: orgGradient
            GradientStop { position: 0.0; color: "darkorange" }
        }

        property var popGradient: Gradient {
            id: whiteGradient
            GradientStop { position: 0.0; color: "white" }
            GradientStop { position: 1.0; color: "#EDF2FF" }
        }

        anchors.bottom: downTextArea.top
        anchors.bottomMargin: 5
        width: mainWindow.width
        height: mainWindow.btnAreaH
        radius: 12
        border.color: "#6C6F85"
        border.width: 2

        Text {
            id: textArea
            text: qsTr(btnText)
            color:"white"
            wrapMode: Text.WordWrap
            width: parent.width
            height: mainWindow.btnAreaH
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            lineHeight: 0.9
            font.pointSize: pointSize
            //font.pixelSize: 18
        }

        property int secsSinceEpoch: 0   //如果需要精确一些，使用毫秒

        MouseArea
        {
            anchors.fill: parent;
            hoverEnabled: false;
            onClicked:
            {
                if (enablePressed)
                {
                    if (parseInt(new Date / 1000) - selectBtn.secsSinceEpoch >= 2)
                    {
                        mainWindow.status = mainWindow.status ? false : true
                        mainWindow.showStatusChange(mainWindow.objectid, 0 , 0, mainWindow.status)
                        //console.log("selectBtn released > 2s")
                    }
                }
                else
                {
                    mainWindow.status = mainWindow.status ? false : true
                    var popH = mainWindow.status ? popAreaH : btnAreaH
                    mainWindow.showStatusChange(mainWindow.objectid, 0 , popH, mainWindow.status)
                }

                if (enablePopArea)
                {
                    selectBtn.gradient = mainWindow.status ? whiteGradient : gradientBtn
                    textArea.color = mainWindow.status ? "black" : "white"
                    backgroundArea.visible = mainWindow.status ? true : false
                    selectBtn.border.color = mainWindow.status ? "white" : "#6C6F85"
                    mainWindow.width = mainWindow.status ? mainWindow.width + 10 : mainWindow.width - 10
                    mainWindow.anchors.leftMargin = mainWindow.status ?
                        mainWindow.anchors.leftMargin - 5 : mainWindow.anchors.leftMargin + 5
                }
                else
                {
                    selectBtn.gradient = gradientBtn
                    textArea.color = "white"
                }
            }
            onPressed: {
                selectBtn.color = "darkorange"
                selectBtn.gradient = orgGradient
                textArea.color = "black"
                selectBtn.secsSinceEpoch = parseInt(new Date / 1000)
                //console.log("selectBtn time: " + selectBtn.secsSinceEpoch)
            }
            onReleased: {
                if (1) return

                if (parseInt(new Date / 1000) - selectBtn.secsSinceEpoch >= 2)
                {
                    mainWindow.showStatusChange(mainWindow, 0 , 0, true)
                    //console.log("selectBtn released > 2s")
                }
            }
        }
    }

    Rectangle {
        id: downTextArea
        anchors.left: selectBtn.left
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 5
        width: selectBtn.width
        color: "transparent"
        height: 15
        Text {
            text: qsTr(downText)
            color:"white"
            anchors.centerIn: parent
            font.pointSize: pointSize - 2
            //font.pixelSize: pointSize
        }
    }
}
