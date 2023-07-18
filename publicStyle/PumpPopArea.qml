import QtQuick 2.0

Rectangle {
    id: mainWindow

    visible: false
    radius: 12

    readonly property int leftBtnMargin: 50
    readonly property int pointSize: 14

    Connections {
        target: publicData
        onSetRTracData: {
            if (value === 0)
                btnSelectStatus(r_off, 8)
            else
                btnSelectStatus(r_on, 7)
        }
    }

    onVisibleChanged: {
        if(visible)
            getDate.start()
        else
            getDate.stop()
    }

    function btnSelectStatus(id, flag)
    {
        id.selected = id.selected ? false : true
        switch(flag)
        {
            case 1:
                catheterSelectBg.visible = id.selected
                break
            case 2:
                r_TracSelectBg.visible = id.selected
                break
            case 7:
                r_on.curColor = "white"
                r_on.textColor = "black"
                r_off.curColor = r_off.orgColor
                r_off.textColor = "white"
                r_TracBtnText.text = "On"
                //publicData.sendAlarmMessage(2, "Enable Auto R-Trac");
                break
            case 8:
                r_off.curColor = "white"
                r_off.textColor = "black"
                r_on.curColor = r_on.orgColor
                r_on.textColor = "white"
                r_TracBtnText.text = "Off"
                //publicData.sendAlarmMessage(2, "Disable Auto R-Trac");
                break
        }
    }

    Component.onCompleted: {
        catheterAlarmBtn.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        r_TracBtn.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        dateBtn.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        networkBtn.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        r_on.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        r_off.btnSelectStatus.connect(mainWindow.btnSelectStatus)
    }

/** ++++++++++++++Catheter++++++++++++++++++++  */

    Rectangle {
        id: catheterAlarmArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height*0.2

        color: "transparent"
        radius: mainWindow.radius

        property string tipeText: "Catheter         Alarms On"
        property string alarmText: "Refer to Operator's Manual Before Deactivating"

        Rectangle {
            id: catheterSelectBg

            width: 450
            radius: 12
            visible: false
            anchors.left: catheterAlarmBtn.left
            anchors.top: catheterAlarmBtn.top
            anchors.bottom: catheterAlarmBtn.bottom

            onVisibleChanged: {
                catheterAlarmBtn.borderColor = visible ? "#3E4261" : catheterAlarmBtn.orgBorderColor
                catheterAlarmBtn.textColor = visible ? "white" : "black"
                catheterAlarmBtn.curColor = visible ? "#3E4261" : catheterAlarmBtn.orgColor
                catheterBtnText.visible = !visible
                catheterAlarmImage.visible = !visible
                catheterAlarmText.visible = !visible
            }

            color: "#3E4261"
        }

        OptionButton {
            id: catheterAlarmBtn

            btnID: 1
            anchors.top: parent.top
            anchors.topMargin: (catheterAlarmArea.height - height)/2
            marginH: mainWindow.leftBtnMargin
            width: 160 + marginH
            height: catheterAlarmArea.height * 0.7
            text: "Catheter         Alarms"
            pointSize: mainWindow.pointSize
            textMargin: 0.9
        }

        Text {
            id: catheterBtnText
            text: catheterAlarmArea.tipeText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: catheterAlarmBtn.right
            anchors.leftMargin: 30
            anchors.top: catheterAlarmBtn.top
            height: catheterAlarmBtn.height
            width: 100
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }

        Image {
            id: catheterAlarmImage
            anchors.left: catheterBtnText.right
            anchors.leftMargin: 70
            anchors.top: catheterAlarmBtn.top
            cache: false
            width: 50
            height: catheterAlarmBtn.height
            source: "qrc:/windowPicture/upPopButton.png"
        }

        Text {
            id: catheterAlarmText
            text: catheterAlarmArea.alarmText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            anchors.left: catheterAlarmImage.right
            anchors.leftMargin: 25
            anchors.top: catheterAlarmBtn.top
            height: catheterAlarmBtn.height
            width: catheterAlarmArea.width - catheterAlarmImage.width - catheterBtnText.width - catheterAlarmBtn.width
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }
    }

/** ++++++++++++++R-Trac++++++++++++++++++++  */

    Rectangle {
        id: r_TracArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: catheterAlarmArea.bottom
        height: parent.height*0.2

        color: "transparent"
        radius: mainWindow.radius

        property string tipeText: "Off"
        property string alarmText: ""

        Rectangle {
            id: r_TracSelectBg

            width: 450
            radius: 12
            visible: false
            anchors.left: r_TracBtn.left
            anchors.top: r_TracBtn.top
            anchors.bottom: r_TracBtn.bottom

            onVisibleChanged: {
                r_TracBtn.borderColor = visible ? "#3E4261" : r_TracBtn.orgBorderColor
                r_TracBtn.textColor = visible ? "white" : "black"
                r_TracBtn.curColor = visible ? "#3E4261" : r_TracBtn.orgColor
                r_TracBtnText.visible = !visible
                r_TracImage.visible = !visible
                r_TracText.visible = !visible
            }

            OptionButton {
                id: r_on

                btnID: 7
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 5
                anchors.leftMargin: 200
                marginH: 0
                width: 100
                height: parent.height - 10
                text: "On"
                textColor: "white"
                orgColor: "#3E4261"
                curColor: "#3E4261"
                btnRadius: 8
                pointSize: mainWindow.pointSize
            }

            OptionButton {
                id: r_off

                btnID: 8
                anchors.top: parent.top
                anchors.left: r_on.right
                anchors.topMargin: 5
                anchors.leftMargin: 30
                marginH: 0
                width: 100
                height: parent.height - 10
                text: "Off"
                textColor: "black"
                orgColor: "#3E4261"
                curColor: "white"
                btnRadius: 8
                pointSize: mainWindow.pointSize
            }

            color: "#3E4261"
        }

        OptionButton {
            id: r_TracBtn

            btnID: 2
            anchors.top: parent.top
            anchors.topMargin: (r_TracArea.height - height)/2
            marginH: mainWindow.leftBtnMargin
            width: 160 + marginH
            height: r_TracArea.height * 0.7
            text: "R-Trac"
            pointSize: mainWindow.pointSize
        }

        Text {
            id: r_TracBtnText
            text: r_TracArea.tipeText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: r_TracBtn.right
            anchors.leftMargin: 30
            anchors.top: r_TracBtn.top
            height: r_TracBtn.height
            width: 100
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }

        Image {
            id: r_TracImage
            anchors.left: r_TracBtnText.right
            anchors.leftMargin: 70
            anchors.top: r_TracBtn.top
            cache: false
            width: 50
            height: r_TracBtn.height
            source: ""
        }

        Text {
            id: r_TracText
            text: r_TracArea.alarmText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            anchors.left: r_TracImage.right
            anchors.leftMargin: 25
            anchors.top: r_TracBtn.top
            height: r_TracBtn.height
            width: r_TracArea.width - r_TracImage.width - r_TracBtnText.width - r_TracBtn.width
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }
    }

/** +++++++++++++++Time&Data++++++++++++++++++++  */

    Timer {
        id: getDate
        interval: 1000
        running: false
        repeat: true
        onTriggered: {
            getDateTime()
        }
    }

    function getDateTime()
    {
        var time_date = new Date()
        dateArea.time_text = Qt.formatDateTime(time_date,"HH:mm:ss")
        dateArea.date_text = Qt.formatDateTime(time_date,"yyyy-MM-dd")
    }

    Rectangle {
        id: dateArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: r_TracArea.bottom
        height: parent.height*0.4

        color: "transparent"
        radius: mainWindow.radius

        property string time_text: "00:00:00"
        property string date_text: "1970-01-01"

        OptionButton {
            id: dateBtn

            btnID: 3
            anchors.top: parent.top
            anchors.topMargin: (dateArea.height - height)/2
            marginH: mainWindow.leftBtnMargin
            width: 160 + marginH
            height: dateArea.height * 0.7
            text: "Set Time
&
Date"
            pointSize: mainWindow.pointSize
        }

        Text {
            id: timeText
            text: dateArea.time_text
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: dateBtn.right
            anchors.leftMargin: 30
            anchors.top: dateBtn.top
            height: dateBtn.height
            width: 100
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }

        Text {
            id: dateText
            text: dateArea.date_text
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            anchors.left: timeText.right
            anchors.leftMargin: 50
            anchors.top: dateBtn.top
            height: dateBtn.height
            width: dateArea.width - timeText.width - dateBtn.width
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }
    }

/** ++++++++++++++++Network++++++++++++++++++++  */

    Rectangle {
        id: networkArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: dateArea.bottom
        height: parent.height*0.2

        color: "transparent"
        radius: mainWindow.radius

        property string tipeText: "Off"
        property string alarmText: ""

        OptionButton {
            id: networkBtn

            btnID: 4
            anchors.top: parent.top
            anchors.topMargin: (r_TracArea.height - height)/2
            marginH: mainWindow.leftBtnMargin
            width: 160 + marginH
            height: r_TracArea.height * 0.7
            text: "Network    Connections"
            pointSize: mainWindow.pointSize
        }

        Text {
            id: networkBtnText
            text: networkArea.tipeText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: networkBtn.right
            anchors.leftMargin: 30
            anchors.top: networkBtn.top
            height: networkBtn.height
            width: 100
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }

        Image {
            id: networkImage
            anchors.left: networkBtnText.right
            anchors.leftMargin: 70
            anchors.top: networkBtn.top
            cache: false
            width: 50
            height: networkBtn.height
            source: ""
        }

        Text {
            id: networkText
            text: networkArea.alarmText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            anchors.left: networkImage.right
            anchors.leftMargin: 25
            anchors.top: networkBtn.top
            height: networkBtn.height
            width: networkArea.width - networkImage.width - networkBtnText.width - networkBtn.width
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }
    }
}
