import QtQuick 2.12
import "../publicStyle"

Item {
    id: menuWindow

    property int pointSize: 18
    property int currentBtn: 0x00

    property int autoBtnx: 40

    Rectangle {
        id: menu
        width: menuWindow.width
        height: menuWindow.height
        color: "#b0c4de"

        readonly property string modelName: "run_mode"

        gradient: Gradient {
            GradientStop { position: 0.0; color: "lightsteelblue" }
            //GradientStop { position: 0.5; color: "lightslategraY" }
            GradientStop { position: 1.0; color: "dodgerblue" }
        }

        border.width: 5
        radius: 10

        Connections {
            target: publicData
            onRestoreMenuStatus: {
                start.color = flag ? "#72A1A8" : start.color
                standbyBtn.color = flag ? standbyBtn.color : "#E9E5B1"
                start.enable_click = flag ? true : false
                standbyBtn.enable_click = flag ? false : true
            }

            onSetRunMode: {
                menu.setColorStyle(value+1)
            }
        }

        function setColorStyle(num) {
            if (num & 0x01)
            {
                autoBtn.color = "#413F3B"
                semiAutoBtn.color = "lightsteelblue"
                calibratePressure.color = "lightsteelblue"
            }
            else if (num & 0x02)
            {
                autoBtn.color = "lightsteelblue"
                semiAutoBtn.color = "#413F3B"
                calibratePressure.color = "lightsteelblue"
            }
            else if (num & 0x04)
            {
                autoBtn.color = "lightsteelblue"
                semiAutoBtn.color = "lightsteelblue"
                calibratePressure.color = "#413F3B"
            }
            if (num & 0x10)
            {
                standbyBtn.color = "#E9E5B1"
                start.color = "#413F3B"
            }
            else if (num & 0x20)
            {
                standbyBtn.color = "#413F3B"
                start.color = "#72A1A8"
            }
        }

        Rectangle {
            id: autoBtn
            x:  autoBtnx
            anchors.verticalCenter: parent.verticalCenter
            width: 175
            height: parent.height/2
            color: "lightsteelblue"
            radius: 39
            border.width: 2
            antialiasing: true
            Text {
                text: qsTr("Auto")
                anchors.centerIn: parent
                color:"white"

                font.pointSize: pointSize
            }

            MouseArea
            {
                anchors.fill: parent;
                //控制开关，如果开启，entered与exited信号会接收，否则不会接收
                hoverEnabled: false;
                onClicked:
                {
                    //console.log(qsTr("按钮被按了"))
                    menu.setColorStyle(0x01)
                    senderdata.setTriggerMode(menu.modelName, 0)
                }
                onPressed: {
                    autoBtn.color = "darkorange"
                    console.log("autoBtn pressed")
                }
                onReleased: {
                     console.log("autoBtn released")
                }
            }
        }

        Rectangle {
            id: semiAutoBtn
            anchors.left: autoBtn.right
            anchors.leftMargin: parent.width / 15
            anchors.verticalCenter: parent.verticalCenter
            width: 175
            height: parent.height/2
            color: "lightsteelblue"
            radius: 39
            border.color: "red"
            border.width: 2
            antialiasing: true

            Text {
                text: qsTr("Semi Auto")
                anchors.centerIn: parent
                color:"white"

                font.pointSize: pointSize
            }
            MouseArea
            {
                anchors.fill: parent;
                hoverEnabled: false;
                onClicked:
                {
                    menu.setColorStyle(0x02)
                    senderdata.setTriggerMode(menu.modelName, 1)
                }
                onPressed: {
                    semiAutoBtn.color = "darkorange"
                }

            }
        }

        Rectangle {
            id: calibratePressure
            anchors.left: semiAutoBtn.right
            anchors.leftMargin: parent.width / 15
            anchors.verticalCenter: parent.verticalCenter
            width: 175
            height: parent.height/2
            color: "lightsteelblue"
            radius: 39
            border.color: "red"
            border.width: 2
            antialiasing: true
            Text {
                text: qsTr("Calibrate Pressure")
                color:"white"
                wrapMode: Text.WordWrap
                width: parent.width
                horizontalAlignment: Text.AlignHCenter
                y: 4
                lineHeight: 0.9
                font.pointSize: pointSize
                //font.pixelSize: 20
            }

            property int secsSinceEpoch: 0   //如果需要精确一些，使用毫秒

            MouseArea
            {
                anchors.fill: parent;
                hoverEnabled: false;
                onClicked:
                {
                    menu.setColorStyle(0x04)
                }
                onPressed: {
                    calibratePressure.color = "darkorange"
                    calibratePressure.secsSinceEpoch = parseInt(new Date / 1000)
                    console.log("calibratePressure time: " + calibratePressure.secsSinceEpoch)
                }
                onReleased: {
                    if (parseInt(new Date / 1000) - calibratePressure.secsSinceEpoch >= 2)
                        senderdata.sensorCalibrate()
                    else
                        console.log("calibratePressure released")
                }
            }
        }

        Rectangle {
            anchors.left: calibratePressure.left
            anchors.top: calibratePressure.bottom
            anchors.topMargin: 5
            width: calibratePressure.width
            height: 25
            color: "transparent"
            Text {
                text: qsTr("Hold 2 sec")
                color:"white"
                anchors.centerIn: parent
                //font.pointSize: pointSize
                font.pixelSize: pointSize
            }
        }

        RoundMouseArea {
            id: standbyBtn
            anchors.right: start.left
            anchors.rightMargin: 30
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height - 40
            height: parent.height - 40
            color: "#E9E5B1"
            radius: 55
            border.width: 2
            antialiasing: true
            enable_click: false
            Text {
                text: qsTr("Standby")
                color:"white"
                anchors.centerIn: parent
                font.pointSize: pointSize
                //font.pixelSize: 20
            }
            onClicked: {
                menu.setColorStyle(0x20)
                senderdata.stopECG(false)
                start.enable_click = true
                standbyBtn.enable_click = false
            }
            onPressed: {
                standbyBtn.color = "darkorange"
            }
        }

        RoundMouseArea {
            id: start
            anchors.right: menu.right
            anchors.rightMargin: autoBtnx
            anchors.verticalCenter: parent.verticalCenter
            width: parent.height - 40
            height: parent.height - 40
            color: "#72A1A8"
            radius: 55
            border.width: 2
            antialiasing: true
            enable_click: true

            Timer {
                id: twinkleTimer
                interval: 1000
                running: false
                repeat: true
                onTriggered: {
                    twinkleArea.opacity_value = twinkleArea.opacity_value ? false : true
                }
            }

            Rectangle {
                id: twinkleArea

                anchors.fill: parent
                anchors.margins: 2

                property bool opacity_value: true

                color: "transparent"
                radius: 55
                border.color: "darkorange"
                border.width: 3
                opacity: 0

                OpacityAnimator{
                    target: twinkleArea
                    from: 0
                    to: 1
                    duration: 500
                    running: false/*twinkleArea.opacity_value*/
                }
                OpacityAnimator{
                    target: twinkleArea
                    from: 1
                    to: 0
                    duration: 500
                    running: false/*!twinkleArea.opacity_value*/
                }
            }

            Text {
                text: qsTr("Start")
                color:"white"
                anchors.centerIn: parent
                font.pointSize: pointSize
                //font.pixelSize: 20
            }
            onClicked: {
                menu.setColorStyle(0x10)
                senderdata.startECG(true)
                start.enable_click = false
                standbyBtn.enable_click = true
            }
            onPressed: {
                start.color = "darkorange"
            }
        }
    }
}
