import QtQuick 2.0
import QtMultimedia 5.8
import "../publicStyle"

Item {
    id: statusWindow

    signal startMaskScreen(bool flag)
    signal optionViewPopChange(var flag)

    SoundEffect {
        id: playSound
        source: "qrc:/windowPicture/phone.wav"
    }

    function showStatusChange(id, p_opacity, p_height, status)
    {
        if (id === lockScreen)
        {
            if (status)
                lockScreen.btnText = "Unlock    Screen"
            else
                lockScreen.btnText = "lock    Screen"

            statusWindow.startMaskScreen(status)
        }
        else if (id === closeMenus)
        {
            if (status)
                closeMenus.btnText = "Close       Menus"
            else
                closeMenus.btnText = "Open       Menus"

            statusWindow.optionViewPopChange(status)
        }
        else if (id === preferences)
        {
            if(status)
            {
                perferPopArea.visible = true;
                refLine.anchors.leftMargin -= 5
                statusWindow.optionViewPopChange(false)
            }
            else
            {
                perferPopArea.visible = false
                refLine.anchors.leftMargin += 5
            }
        }
        else if (id === refLine)
        {
            if(status)
            {
                refLinePopArea.visible = true
                iabFill.anchors.leftMargin -= 5
                statusWindow.optionViewPopChange(false)
            }
            else
            {
                refLinePopArea.visible = false
                iabFill.anchors.leftMargin += 5
            }
            //iabPop.visible = iabPop.visible ? false : true
            senderdata.setMoveLineMode(status)
            //iabPop.height = p_height
        }
        else if (id === augAlarm)
        {
            if(status)
            {
                augAlarmPopArea.visible = true
                printStrip.anchors.leftMargin -= 5
                statusWindow.optionViewPopChange(false)
            }
            else
            {
                augAlarmPopArea.visible = false
                printStrip.anchors.leftMargin += 5
            }
        }
        else if (id === iabFill)
        {
            //iabPop.visible = iabPop.visible ? false : true
            senderdata.setIABFill()
            //iabPop.height = p_height
        }
        else if (id === printStrip)
        {
            //iabPop.visible = iabPop.visible ? false : true
            senderdata.setWriteFile(status)
            //iabPop.height = p_height
        }
        else if (id === freezeDisplay)
        {
            playSound.play()

            publicData.setFreezeDisplay(status)
        }
    }

    function setMenuStatus(id, value)
    {
        if (id === 1) //closeMenus
        {
            if (value & 0x1f)
            {
                if (perferPopArea.visible)
                    refLine.anchors.leftMargin += 5;
                if (refLinePopArea.visible)
                    iabFill.anchors.leftMargin += 5;
                if (augAlarmPopArea.visible)
                    printStrip.anchors.leftMargin += 5;
                perferPopArea.visible = false
                refLinePopArea.visible = false
                refLine.hideArea()
                augAlarmPopArea.visible = false
                augAlarm.hideArea()
                preferences.hideArea()

                closeMenus.status = true
                closeMenus.btnText = "Close       Menus"
            }
            else
            {
                closeMenus.status = false
                closeMenus.btnText = "Open       Menus"
            }
        }
    }

    Component.onCompleted: {
        lockScreen.showStatusChange.connect(statusWindow.showStatusChange)
        closeMenus.showStatusChange.connect(statusWindow.showStatusChange)
        preferences.showStatusChange.connect(statusWindow.showStatusChange)
        refLine.showStatusChange.connect(statusWindow.showStatusChange)
        iabFill.showStatusChange.connect(statusWindow.showStatusChange)
        freezeDisplay.showStatusChange.connect(statusWindow.showStatusChange)
        augAlarm.showStatusChange.connect(statusWindow.showStatusChange)
        printStrip.showStatusChange.connect(statusWindow.showStatusChange)
    }

    StatusButton {
        id: lockScreen

        anchors.left: parent.left
        anchors.leftMargin: 47
        anchors.bottom: parent.bottom
        //anchors.bottomMargin: 20

        width: 95
        objectid: lockScreen
        enablePressed: true
        downText: "Hold 2 Sec."
        downTextVisible: true
        btnText: "Lock    Screen"
    }

    StatusButton {
        id: closeMenus

        anchors.left: lockScreen.right
        anchors.leftMargin: lockScreen.anchors.leftMargin
        anchors.bottom: parent.bottom

        width: 95
        objectid: closeMenus
        btnText: "Open       Menus"
    }

    StatusButton {
        id: preferences

        anchors.left: closeMenus.right
        anchors.leftMargin: lockScreen.anchors.leftMargin
        anchors.bottom: parent.bottom

        width: 140
        enablePopArea: true
        objectid: preferences
        popAreaH: 120
        btnText: "Preferences"

        StatusPerferPopArea {
            id: perferPopArea

            x:  lockScreen.x - preferences.x
            anchors.bottom: preferences.bottom
            anchors.bottomMargin: 190 - 5
            height: 400
            width: printStrip.x - lockScreen.x + printStrip.width
        }
    }

    StatusButton {
        id: refLine

        anchors.left: preferences.right
        anchors.leftMargin: lockScreen.anchors.leftMargin
        anchors.bottom: parent.bottom

        width: 95
        objectid: refLine
        enablePopArea: true
        popAreaH: 90

        btnText: "Ref             Line"

        StatusRefLinePopArea {
            id: refLinePopArea
            anchors.left: refLine.left
            anchors.bottom: refLine.bottom
            anchors.bottomMargin: 85
            height: 80
            width: refLine.width + 85
            radius: 50
            first:  "#F3F7FF"
            second: "#EDF2FF"
        }
    }

    StatusButton {
        id: iabFill

        anchors.left: refLine.right
        anchors.leftMargin: lockScreen.anchors.leftMargin
        anchors.bottom: parent.bottom

        width: 95
        objectid: iabFill

        downText: "Hold 2 Sec."
        downTextVisible: true
        btnText: "IAB                Fill"

        enablePressed: true
    }

    StatusButton {
        id: freezeDisplay

        anchors.left: iabFill.right
        anchors.leftMargin: lockScreen.anchors.leftMargin
        anchors.bottom: parent.bottom

        width: 105
        objectid: freezeDisplay

        btnText: "Freeze Display"
    }

    StatusButton {
        id: augAlarm

        anchors.left: freezeDisplay.right
        anchors.leftMargin: lockScreen.anchors.leftMargin
        anchors.bottom: parent.bottom

        width: 125
        objectid: augAlarm
        enablePopArea: true

        btnText: "Aug                  Alarm"

        popAreaH: 90

        /* 可与 refLinePopArea 合并 */
        StatusAugAlarmPopArea {
            id: augAlarmPopArea
            anchors.right: augAlarm.right
            anchors.bottom: augAlarm.bottom
            anchors.bottomMargin: 85
            height: 80
            width: augAlarm.width + 65
            radius: 50
            first:  "#F3F7FF"
            second: "#EDF2FF"
        }
    }

    StatusButton {
        id: printStrip

        anchors.left: augAlarm.right
        anchors.leftMargin: lockScreen.anchors.leftMargin
        anchors.bottom: parent.bottom

        width: 105
        objectid: printStrip



        downText: "Hold for Continuous"
        downTextVisible: true
        btnText: "Print              Strip"
    }

}

