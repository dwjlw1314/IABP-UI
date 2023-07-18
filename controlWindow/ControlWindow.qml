import QtQuick 2.0
import QtQuick.Controls 2.2
import "../publicStyle"

Rectangle {
    id: controlWindow

    function areaBoxChange(id, flag)
    {
        if (id === alarmView)
        {
            if (!flag)
            {
                alarmView.state = "alarmHide"
                alarmView.opacity = 0.0
                alarmBtn.visible = true
            }
            else
            {
                alarmView.state = "alarmShow"
                alarmView.opacity = 1.0
                alarmBtn.visible = false
            }
        }
    }

    MenuView {
        id: menuView
        width: parent.width
        height: 150
    }

    OptionView {
        id: optionView
        //y: menuView.height
        anchors.top: menuView.bottom
        anchors.left: parent.left

        width: parent.width
        height: parent.height - menuView.height - statusView.height // 80 buttonView height

        btnMargin: 56

        onOptionBitChanged: {
            statusView.setMenuStatus(1, optionView.optionBit)
        }
    }

    AlarmView {
        id: alarmView

        objectId: alarmView
        anchors.left: parent.right
        anchors.leftMargin: 550
        anchors.top: optionView.top
        anchors.topMargin: 215
        anchors.right: parent.right
        anchors.rightMargin: optionView.btnMargin
        anchors.bottom: statusView.top
        anchors.bottomMargin: 95

        states: [
            State {
                name: "alarmHide"
                AnchorChanges {
                    target: alarmView;
                    anchors.left: parent.right
                }
            },
            State {
                name: "alarmShow"
                AnchorChanges {
                    target: alarmView;
                    anchors.left: optionView.left
                }
            }
        ]

        transitions: Transition {
            AnchorAnimation {
                duration: 500
            }
        }

        Behavior on opacity { PropertyAnimation {duration: 200} }
    }

    Button {
        id: alarmBtn
        visible: true
        width: 100
        height: 40
        anchors.right: parent.right
        anchors.rightMargin: 50
        anchors.bottom: statusView.top
        anchors.bottomMargin: 10

        background: Rectangle {
            border.width: 2
            radius: 10
            gradient: Gradient {
                GradientStop { position: 0.0; color: "#474B6A" }
                GradientStop { position: 1.0; color: "#777CBC" }
            }
        }

        text: "Alarm Show"
        /** use images */

        onClicked: areaBoxChange(alarmView, true)
    }

    StatusView {
        id: statusView

        anchors.top: optionView.bottom
        anchors.left: optionView.left

        width: parent.width
        height: 80

        onStartMaskScreen: {
            screenMask.visible = flag ? true : false
            //unlockSlider.visible = true
            //console.log("onStartMaskScreen")
        }

        onOptionViewPopChange: {
            optionView.popAllButton(flag)
        }
    }

    Component.onCompleted: {
        alarmView.alarmBoxChange.connect(controlWindow.areaBoxChange)
    }

    /** *************************************************** */

    Button {
        id: screenMask
        visible: false
        width: parent.width
        height: parent.height - statusView.height - 85
        anchors.left: parent.left
        anchors.top: parent.top
        opacity: 0.5
        background: Rectangle {
            border.width: 0
            color: "black"

            gradient: Gradient {
                GradientStop { position: 0.0; color: "black" }
                GradientStop { position: 1.0; color: "transparent" }
            }
        }

        Text {
            text: "Locked Screen"
            color:"white"
            //font.family: "Helvetica"
            anchors.centerIn: parent
            horizontalAlignment: Text.AlignHCenter
            verticalAlignment: Text.AlignVCenter
            font.pointSize: 35
            font.bold : true
            font.italic : true
        }
        /** use images */
    }

    /**
    UnlockSlider {
        id: unlockSlider
        anchors.centerIn: parent
        hint: "滑动解锁"
        visible: false
        sourceLock: "qrc:/windowPicture/left.png"
        sourceUnlock: "qrc:/windowPicture/right.png"
        onUnlock: {
            console.log("screen unlock")
            screenMask.visible = false
            unlockSlider.visible = false
        }
    }

    ShapeRectangle {
        x: 100
        y: 100
        width: 70
        height: 70

        cornersRadius: [0,35,35,0]
        color: "red"
    }

    PopUpButton {
        id: popUpButton
        anchors.bottom: pressKey.top
        anchors.horizontalCenter: pressKey.horizontalCenter
    }

    RoundRectangle {
        id: my
        width: 100
        height: 80
        anchors.centerIn: parent
        //color: "red"
        radius: height/2
        radiusCorners:  Qt.AlignLeft | Qt.AlignBottom
        title:"btn1"
        MouseArea{
            anchors.fill: parent

            onClicked: {
                console.debug("左边点击")
                my.checked = true;
            }

            onPressed: {
                my.color = "gray"
            }

            onReleased: {
                my.color = "yellow"
            }
        }
    }
    */
}

