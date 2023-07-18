import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5

Rectangle {
    id: mainWindow

    visible: true
    radius: 12

    readonly property int pointSize: 14
    readonly property int leftBtnMargin: 50

    function btnSelectStatus(id, flag)
    {
        id.selected = id.selected ? false : true

        switch(flag)
        {
            case 1:
                sweepSpeedArea.visible = id.selected
                break
            case 2:
                balloonSelectBg.visible = id.selected
                break
            case 3:
                ecgMarkerSelectBg.visible = id.selected
                break
            case 8:
                b_on.curColor = "white"
                b_on.textColor = "black"
                b_off.curColor = b_off.orgColor
                b_off.textColor = "white"
                balloonWaveBtnText.text = "On"
                //publicData.sendAlarmMessage(2, "Enable Auto R-Trac");
                break
            case 9:
                b_off.curColor = "white"
                b_off.textColor = "black"
                b_on.curColor = b_on.orgColor
                b_on.textColor = "white"
                balloonWaveBtnText.text = "Off"
                //publicData.sendAlarmMessage(2, "Disable Auto R-Trac");
                break
            case 10:
                e_on.curColor = "white"
                e_on.textColor = "black"
                e_off.curColor = e_off.orgColor
                e_off.textColor = "white"
                ecgMarkerBtnText.text = "On"
                //publicData.sendAlarmMessage(2, "Enable Auto R-Trac");
                break
            case 11:
                e_off.curColor = "white"
                e_off.textColor = "black"
                e_on.curColor = e_on.orgColor
                e_on.textColor = "white"
                ecgMarkerBtnText.text = "Off"
                //publicData.sendAlarmMessage(2, "Enable Auto R-Trac");
                break
        }
    }

    Component.onCompleted: {
        balloonWaveBtn.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        ecgMarkerBtn.btnSelectStatus.connect(mainWindow.btnSelectStatus)

        b_on.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        b_off.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        e_on.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        e_off.btnSelectStatus.connect(mainWindow.btnSelectStatus)

        b_on.curColor = "white"
        e_off.curColor = "white"
    }

    Connections {
        target: publicData
        onSetEcgSpeedData: {
            switch(value)
            {
            case 1:
                speedlow.checked = true; break
            case 2:
                speedmid.checked = true; break
            case 4:
                speedhigh.checked = true; break
            }
        }
    }

/** +++++++++++++++++Sweep Speed+++++++++++++++++++  */

    Rectangle {
        id: sweepSpeedArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        height: parent.height*0.2

        color: "transparent"
        radius: mainWindow.radius

        property string tipeText: "25 mm/sec"
        property string alarmText: ""

        Rectangle {
            id: sweepSelectBg

            width: 450
            radius: 12
            visible: false
            anchors.left: sweepBtn.left
            anchors.top: sweepBtn.top
            anchors.bottom: sweepBtn.bottom

            color: "#3E4261"
        }

        OptionButton {
            id: sweepBtn

            btnID: 1
            anchors.top: parent.top
            anchors.topMargin: (sweepSpeedArea.height - height)/2
            marginH: mainWindow.leftBtnMargin
            width: 160 + marginH
            height: sweepSpeedArea.height * 0.7
            text: "Sweep Speed"
            pointSize: mainWindow.pointSize
        }

        RowLayout {
            id: speedLayout
            anchors.left: sweepBtn.right
            anchors.leftMargin: 60
            width: 130
            height: parent.height

            RadioButton {
                id: speedlow
                text: qsTr("12.5 mm/s")
                font.pointSize: 15
                onClicked: senderdata.setEcgSpeedGain(1);
            }

            RadioButton {
                id: speedmid
                checked: true
                text: qsTr("25 mm/s")
                font.pointSize: 15
                onClicked: senderdata.setEcgSpeedGain(2);
            }

            RadioButton {
                id: speedhigh
                text: qsTr("50 mm/s")
                font.pointSize: 15
                onClicked: senderdata.setEcgSpeedGain(4);
            }
        }

        Text {
            id: sweepBtnText
            visible: false
            text: sweepSpeedArea.tipeText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: sweepBtn.right
            anchors.leftMargin: 30
            anchors.top: sweepBtn.top
            height: sweepBtn.height
            width: 100
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }

        Image {
            id: sweepImage
            anchors.left: sweepBtnText.right
            anchors.leftMargin: 70
            anchors.top: sweepBtn.top
            cache: false
            width: 50
            height: sweepBtn.height
            source: ""
        }

        Text {
            id: sweepText
            text: sweepSpeedArea.alarmText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            anchors.left: sweepImage.right
            anchors.leftMargin: 25
            anchors.top: sweepBtn.top
            height: sweepBtn.height
            width: sweepSpeedArea.width - sweepImage.width - sweepBtnText.width - sweepBtn.width
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }
    }

/** +++++++++++++++++Balloon Waveform+++++++++++++++++++  */

    Rectangle {
        id: balloonWaveArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: sweepSpeedArea.bottom
        height: parent.height*0.2

        color: "transparent"
        radius: mainWindow.radius

        property string tipeText: "On"
        property string alarmText: ""

        Rectangle {
            id: balloonSelectBg

            width: 450
            radius: 12
            visible: false
            anchors.left: balloonWaveBtn.left
            anchors.top: balloonWaveBtn.top
            anchors.bottom: balloonWaveBtn.bottom

            onVisibleChanged: {
                balloonWaveBtn.borderColor = visible ? "#3E4261" : balloonWaveBtn.orgBorderColor
                balloonWaveBtn.textColor = visible ? "white" : "black"
                balloonWaveBtn.curColor = visible ? "#3E4261" : balloonWaveBtn.orgColor
                balloonWaveBtnText.visible = !visible
                balloonWaveImage.visible = !visible
                balloonWaveText.visible = !visible
            }

            OptionButton {
                id: b_on

                btnID: 8
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.topMargin: 5
                anchors.leftMargin: 200
                marginH: 0
                width: 100
                height: parent.height - 10
                text: "On"
                textColor: "black"
                orgColor: "#3E4261"
                curColor: "white"
                btnRadius: 8
                pointSize: mainWindow.pointSize
            }

            OptionButton {
                id: b_off

                btnID: 9
                anchors.top: parent.top
                anchors.left: b_on.right
                anchors.topMargin: 5
                anchors.leftMargin: 30
                marginH: 0
                width: 100
                height: parent.height - 10
                text: "Off"
                textColor: "white"
                orgColor: "#3E4261"
                curColor: "#3E4261"
                btnRadius: 8
                pointSize: mainWindow.pointSize
            }

            color: "#3E4261"
        }

        OptionButton {
            id: balloonWaveBtn

            btnID: 2
            anchors.top: parent.top
            anchors.topMargin: (balloonWaveArea.height - height)/2
            marginH: mainWindow.leftBtnMargin
            width: 160 + marginH
            height: balloonWaveArea.height * 0.7
            text: "Balloon Waveform"
            pointSize: mainWindow.pointSize
        }

        Text {
            id: balloonWaveBtnText
            text: balloonWaveArea.tipeText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: balloonWaveBtn.right
            anchors.leftMargin: 30
            anchors.top: balloonWaveBtn.top
            height: balloonWaveBtn.height
            width: 100
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }

        Image {
            id: balloonWaveImage
            anchors.left: balloonWaveBtnText.right
            anchors.leftMargin: 70
            anchors.top: balloonWaveBtn.top
            cache: false
            width: 50
            height: balloonWaveBtn.height
            source: ""
        }

        Text {
            id: balloonWaveText
            text: balloonWaveArea.alarmText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            anchors.left: balloonWaveImage.right
            anchors.leftMargin: 25
            anchors.top: balloonWaveBtn.top
            height: balloonWaveBtn.height
            width: balloonWaveArea.width - balloonWaveImage.width - balloonWaveBtnText.width - balloonWaveBtn.width
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }
    }

/** +++++++++++++++++ECG Markers+++++++++++++++++++  */

    Rectangle {
        id: ecgMarkerArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: balloonWaveArea.bottom
        height: parent.height*0.2

        color: "transparent"
        radius: mainWindow.radius

        property string tipeText: "Off"
        property string alarmText: ""

        Rectangle {
            id: ecgMarkerSelectBg

            width: 450
            radius: 12
            visible: false
            anchors.left: ecgMarkerBtn.left
            anchors.top: ecgMarkerBtn.top
            anchors.bottom: ecgMarkerBtn.bottom

            onVisibleChanged: {
                ecgMarkerBtn.borderColor = visible ? "#3E4261" : ecgMarkerBtn.orgBorderColor
                ecgMarkerBtn.textColor = visible ? "white" : "black"
                ecgMarkerBtn.curColor = visible ? "#3E4261" : ecgMarkerBtn.orgColor
                ecgMarkerBtnText.visible = !visible
                ecgMarkerImage.visible = !visible
                ecgMarkerText.visible = !visible
            }

            OptionButton {
                id: e_on

                btnID: 10
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
                id: e_off

                btnID: 11
                anchors.top: parent.top
                anchors.left: e_on.right
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
            id: ecgMarkerBtn

            btnID: 3
            anchors.top: parent.top
            anchors.topMargin: (ecgMarkerArea.height - height)/2
            marginH: mainWindow.leftBtnMargin
            width: 160 + marginH
            height: ecgMarkerArea.height * 0.7
            text: "ECG Markers"
            pointSize: mainWindow.pointSize
        }

        Text {
            id: ecgMarkerBtnText
            text: ecgMarkerArea.tipeText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
            anchors.left: ecgMarkerBtn.right
            anchors.leftMargin: 30
            anchors.top: ecgMarkerBtn.top
            height: ecgMarkerBtn.height
            width: 100
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }

        Image {
            id: ecgMarkerImage
            anchors.left: ecgMarkerBtnText.right
            anchors.leftMargin: 70
            anchors.top: ecgMarkerBtn.top
            cache: false
            width: 50
            height: ecgMarkerBtn.height
            source: ""
        }

        Text {
            id: ecgMarkerText
            text: ecgMarkerArea.alarmText
            wrapMode: Text.WordWrap
            verticalAlignment: Text.AlignVCenter
            anchors.left: ecgMarkerImage.right
            anchors.leftMargin: 25
            anchors.top: ecgMarkerBtn.top
            height: ecgMarkerBtn.height
            width: ecgMarkerArea.width - ecgMarkerImage.width - ecgMarkerBtnText.width - ecgMarkerBtn.width
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }
    }

/** +++++++++++++++++Inflation Interval+++++++++++++++++++  */

    Rectangle {
        id: inflationArea

        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: ecgMarkerArea.bottom
        height: parent.height*0.2

        color: "transparent"
        radius: mainWindow.radius

        property string tipeText: "Press to temporarily display Inflation Markers"

        OptionButton {
            id: inflationBtn

            btnID: 4
            anchors.top: parent.top
            anchors.topMargin: (inflationArea.height - height)/2
            marginH: mainWindow.leftBtnMargin
            width: 160 + marginH
            height: inflationArea.height * 0.7
            text: "Inflation     Interval"
            pointSize: mainWindow.pointSize
        }

        Text {
            id: inflationBtnText
            text: inflationArea.tipeText
            verticalAlignment: Text.AlignVCenter
            anchors.left: inflationBtn.right
            anchors.leftMargin: 30
            anchors.top: inflationBtn.top
            height: inflationBtn.height
            font.pointSize: mainWindow.pointSize
            lineHeight: 0.8
        }
    }

/** +++++++++++++++++Brightness+++++++++++++++++++  */

        Rectangle {
            id: brightArea

            anchors.left: parent.left
            anchors.right: parent.right
            anchors.top: inflationArea.bottom
            height: parent.height*0.2

            color: "transparent"
            radius: mainWindow.radius

            property string tipeText: "High"

            OptionButton {
                id: brightBtn

                btnID: 5
                anchors.top: parent.top
                anchors.topMargin: (brightArea.height - height)/2
                marginH: mainWindow.leftBtnMargin
                width: 160 + marginH
                height: brightArea.height * 0.7
                text: "Brightness"
                pointSize: mainWindow.pointSize
            }

            Text {
                id: brightBtnText
                text: brightArea.tipeText
                verticalAlignment: Text.AlignVCenter
                anchors.left: brightBtn.right
                anchors.leftMargin: 30
                anchors.top: brightBtn.top
                height: brightBtn.height
                font.pointSize: mainWindow.pointSize
                lineHeight: 0.8
            }
        }
}
