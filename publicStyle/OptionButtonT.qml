import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id: mainWindow
    anchors.left: parent.left
    antialiasing: true

    property int marginH: 40
    property int pointSize: 14
    property string text: "Inflation"  //"Deflation"

    Connections {
        target: publicData
        onSetTimeData: {
            if (mainWindow.text === "Inflation")
                globalArea.currentIndex = value >> 8 & 0xf
            else
                globalArea.currentIndex = value & 0xf
        }
    }

    Rectangle {
        id: globalArea

        width: mainWindow.width
        height: mainWindow.height

        color: "transparent"

        property bool flag: true
        property int currentIndex: 50
        property int totalIndex: 100

        property int secsSinceEpoch: 0

        Text {
            id: textArea
            text: mainWindow.text
            color:"black"
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: pointSize
            height: globalArea.height*0.3
        }

        Timer {
            id: fastChange
            interval: 100
            running: false
            repeat: true
            onTriggered: {
                //console.log(globalArea.currentIndex)
                if (parseInt(new Date / 1000) - globalArea.secsSinceEpoch > 1)
                {
                    if (globalArea.flag && globalArea.currentIndex < globalArea.totalIndex)
                    {
                        globalArea.currentIndex = globalArea.currentIndex+3 > globalArea.totalIndex
                                ? globalArea.totalIndex : globalArea.currentIndex+3
                        senderdata.setTriggerMode(mainWindow.text, globalArea.currentIndex)
                    }
                    else if (!globalArea.flag && globalArea.currentIndex > 0)
                    {
                        globalArea.currentIndex = globalArea.currentIndex - 3 < 0
                                ? 0 : globalArea.currentIndex - 3
                        senderdata.setTriggerMode(mainWindow.text, globalArea.currentIndex)
                    }
                }
            }
        }

        RoundMouseArea {
            id: leftBtn
            anchors.top: textArea.bottom
            anchors.topMargin: 5
            anchors.left: parent.left
            anchors.leftMargin: 15
            width: 40
            height: 40
            color: "transparent"
            radius: 55

            Image {
                id:leftImage
                anchors.fill: parent
                cache: false
                source: "qrc:/windowPicture/left.png"
            }

            onClicked: {
                //console.log("OptionButtonT leftBtn clicked")
                fastChange.stop()
                if (globalArea.currentIndex > 0)
                    globalArea.currentIndex--

                senderdata.setTriggerMode(mainWindow.text, globalArea.currentIndex)
            }
            onPressed: {
                //console.log("OptionButtonT leftBtn pressed")
                globalArea.flag = false
                globalArea.secsSinceEpoch = parseInt(new Date / 1000)
                fastChange.start()
            }
        }

        RoundMouseArea {
            id: rightBtn
            anchors.top: textArea.bottom
            anchors.topMargin: leftBtn.anchors.topMargin
            anchors.right: parent.right
            anchors.rightMargin: leftBtn.anchors.leftMargin
            width: 40
            height: 40
            color: "transparent"
            radius: 55
            //border.width: 1

            Image {
                id: rightImage
                anchors.fill: parent
                cache: false
                source: "qrc:/windowPicture/right.png"
            }

            onClicked: {
                //mainWindow.getValue(true)
                //console.log("rightBtn clicked")
                fastChange.stop()
                if (globalArea.currentIndex < globalArea.totalIndex)
                    globalArea.currentIndex++
                senderdata.setTriggerMode(mainWindow.text, globalArea.currentIndex)
            }
            onPressed: {
                //console.log("rightBtn pressed")
                globalArea.flag = true
                globalArea.secsSinceEpoch = parseInt(new Date / 1000)
                fastChange.start()
            }
        }
    }
}
