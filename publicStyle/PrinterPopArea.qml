import QtQuick 2.0
import QtQuick.Controls 2.2

Rectangle {
    id: mainWindow

    visible: false
    radius: 12

    property int fontSize: 16

    Rectangle {
        id: left
        width: parent.width / 2
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top
        color: "transparent"

        Column {
            anchors.centerIn: parent
            spacing: 10

            Text {
               id: soundText
               text: qsTr("当前音量值： 100%")
               font.pixelSize: fontSize
               color: "green"
            }

            Slider{
                id: soundSlider
                stepSize: 1
                from: 0
                to: 100
                value: 100
                live: false
                orientation:Qt.Horizontal
                snapMode:"SnapAlways"
                onValueChanged: {
                    soundText.text = "当前音量值： " + value.toFixed(0)+"%";
                    senderdata.setSystemVolume(value.toFixed(0));
                }
            }

           Text {
                id: showScreen
                text: qsTr("显示屏亮度： 200")
                font.pixelSize: fontSize
                color: "green"
           }
           Slider{
                from: 0
                to: 255
                stepSize: 1
                value: 200
                live: false
                orientation:Qt.Horizontal
                snapMode:"SnapAlways"
                onValueChanged: {
                    showScreen.text = "显示屏亮度： " + value.toFixed(0);
                    senderdata.setShowBrightness(value.toFixed(0));
                }
           }

           Text {
                id: controlScreen
                text: qsTr("控制屏亮度： 200")
                font.pixelSize: fontSize
                color: "green"
           }
           Slider{
                from: 0
                to: 255
                stepSize: 1
                value: 200
                live: false
                orientation:Qt.Horizontal
                snapMode:"SnapAlways"
                onValueChanged: {
                    controlScreen.text = "控制屏亮度： " + value.toFixed(0);
                    senderdata.setControlBrightness(value.toFixed(0));
                }
            }
        }
    }

    Rectangle {
        id: right
        width: parent.width / 2
        height: parent.height
        anchors.left: left.right
        anchors.top: parent.top
        color: "transparent"

        property string showAllEcgWaveText: "Single Lead"

        function btnSelectStatus(id, flag)
        {
            if (id === showAllEcgWave)
            {
                id.curColor = id.orgColor
                id.selected = id.selected ? false : true
                if (id.selected)
                {
                    showAllEcgWaveText = "All Lead"
                    senderdata.setShowMoreEcgWave(1)
                }
                else
                {
                    showAllEcgWaveText = "Single Lead"
                    senderdata.setShowMoreEcgWave(0)
                }
            }
        }

        Component.onCompleted: {
            showAllEcgWave.btnSelectStatus.connect(right.btnSelectStatus)
        }

        Column {
            anchors.centerIn: parent
            spacing: 10

            OptionButton {
                id: showAllEcgWave
                pointSize: 12
                width: right.width * 0.35
                height: right.height * 0.13
                text: right.showAllEcgWaveText
                btnID: 1
            }
        }
    }
}
