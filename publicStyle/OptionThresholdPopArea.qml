import QtQuick 2.0

Item {
    id: mainWindow

    property int pointSize: 14
    property bool threshold_visable: false
    /* 根据实际值设置 */
    property int curThreshold: (maxThreshold+minThreshold)*.5
    property int minThreshold: 7
    property int maxThreshold: 30

    Rectangle {
        id: btnArea

        anchors.fill: parent
        radius: 12

        visible: threshold_visable

        gradient: Gradient {
            GradientStop { position: 0.0; color: "#F3F7FF" }
            //GradientStop { position: 0.5; color: "lightslategraY" }
            GradientStop { position: 1.0; color: "#D1D3F8" }
        }

        readonly property string modelName: "Thresholds"
        property bool flag: true
        property int secsSinceEpoch: 0

        Timer {
            id: fastChange
            interval: 100
            running: false
            repeat: true
            onTriggered: {
                if (parseInt(new Date / 1000) - btnArea.secsSinceEpoch >= 1)
                {
                    if (btnArea.flag && mainWindow.curThreshold < mainWindow.maxThreshold)
                        mainWindow.curThreshold++
                    else if (!btnArea.flag && mainWindow.curThreshold > minThreshold)
                        mainWindow.curThreshold--
                }
            }
        }

        RoundMouseArea {
            id: upArea

            anchors.left: btnArea.left
            anchors.leftMargin: (btnArea.width - upArea.width)*0.5
            anchors.top: btnArea.top
            anchors.topMargin: 2

            width: btnArea.width*0.4
            height: btnArea.width*0.4
            color: "transparent"
            radius: 55
            border.width: 2

            Image {
                id: upImage
                anchors.fill: parent
                cache: false
                source: "qrc:/windowPicture/up.png"
            }

            onClicked: {
                fastChange.stop()
                if (mainWindow.curThreshold < mainWindow.maxThreshold)
                    mainWindow.curThreshold += 1

                //console.log("curThreshold -->" + mainWindow.curThreshold)
                senderdata.setTriggerMode(btnArea.modelName, mainWindow.curThreshold)
            }
            onPressed: {
                //console.log("OptionAugPopArea.qml upArea onPressed -->")
                btnArea.flag = true
                btnArea.secsSinceEpoch = parseInt(new Date / 1000)
                fastChange.start()
            }
        }

        Rectangle {
            anchors.centerIn: parent
            color:"black"
            width: btnArea.width*0.35
            height: btnArea.width*0.35
            anchors.top: upArea.top
            anchors.left: upArea.left
            anchors.margins: 10
            radius: 8
            border.width: 2
            border.color: "#D1D3F8"

            Text {
                id: midText
                text: mainWindow.curThreshold
                anchors.centerIn: parent
                color:"white"
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: pointSize
            }
        }

        RoundMouseArea {
            id: downArea

            anchors.left: btnArea.left
            anchors.leftMargin: upArea.anchors.leftMargin
            anchors.bottom: btnArea.bottom
            anchors.bottomMargin: 2

            width: upArea.width
            height: upArea.height
            color: "transparent"
            radius: 55
            border.width: upArea.border.width

            Image {
                id: downImage
                anchors.fill: parent
                cache: false
                source: "qrc:/windowPicture/down.png"
            }

            onClicked: {
                //console.log("OptionAugPopArea.qml downArea onClicked -->")
                fastChange.stop()
                if (mainWindow.curThreshold > mainWindow.minThreshold)
                    mainWindow.curThreshold -= 1

                //console.log("curThreshold -->" + mainWindow.curThreshold)
                senderdata.setTriggerMode(btnArea.modelName, mainWindow.curThreshold)
            }
            onPressed: {
                //console.log("OptionAugPopArea.qml downArea onPressed -->")
                btnArea.flag = false
                btnArea.secsSinceEpoch = parseInt(new Date / 1000)
                fastChange.start()
            }
        }
    }
}
