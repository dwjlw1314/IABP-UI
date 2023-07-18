import QtQuick 2.0

Rectangle {
    id: mainWindow

    property int pointSize: 14

    property int minThreshold: 20
    property int maxThreshold: 100

    /* 根据实际值设置 */
    property int curThreshold: (maxThreshold+minThreshold)*.5

    readonly property string modelName: "AugAlarm"
    property bool flag: true
    property int secsSinceEpoch: 0

    property string first:  "#F3F7FF"
    property string second: "#D1D3F8"

    gradient: Gradient {
        GradientStop { position: 0.0; color: first }
        //GradientStop { position: 0.5; color: "lightslategraY" }
        GradientStop { position: 1.0; color: second }
    }

    radius: 12
    visible: false

    Timer {
        id: fastChange
        interval: 100
        running: false
        repeat: true
        onTriggered: {
            if (parseInt(new Date / 1000) - mainWindow.secsSinceEpoch >= 1)
            {
                if (mainWindow.flag && mainWindow.curThreshold < mainWindow.maxThreshold)
                    mainWindow.curThreshold++
                else if (!mainWindow.flag && mainWindow.curThreshold > minThreshold)
                    mainWindow.curThreshold--
            }
        }
    }

    RoundMouseArea {
        id: leftArea

        anchors.left: mainWindow.left
        anchors.leftMargin: 15
        anchors.top: mainWindow.top
        anchors.topMargin: (mainWindow.height - leftArea.height)/2

        width: mainWindow.height*0.6
        height: mainWindow.height*0.6
        color: "transparent"
        radius: 55
        border.width: 2

        Image {
            id:leftImage
            anchors.fill: parent
            cache: false
            source: "qrc:/windowPicture/up.png"
        }

        onClicked: {
            //console.log("OptionAugPopArea.qml upArea onClicked -->")
            fastChange.stop()
            if (mainWindow.curThreshold < mainWindow.maxThreshold)
                mainWindow.curThreshold += 1
            //senderdata.setTriggerMode(mainWindow.modelName, mainWindow.curThreshold - mainWindow.minThreshold)
            senderdata.modifyAugAlarmData(mainWindow.curThreshold - mainWindow.minThreshold)
        }
        onPressed: {
            //console.log("OptionAugPopArea.qml upArea onPressed -->")
            mainWindow.flag = true
            mainWindow.secsSinceEpoch = parseInt(new Date / 1000)
            fastChange.start()
        }
        //onReleased: fastChange.stop()
    }

    Rectangle {
        anchors.centerIn: parent
        color:"black"
        width: parent.width*0.25
        height: parent.width*0.2
        anchors.left: leftArea.right
        anchors.top: leftArea.top
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
        id: rightArea

        anchors.right: mainWindow.right
        anchors.rightMargin: 15
        anchors.top: mainWindow.top
        anchors.topMargin: (mainWindow.height - rightArea.height)/2

        width: leftArea.width
        height: leftArea.height
        color: "transparent"
        radius: 55
        border.width: leftArea.border.width

        Image {
            id: rightImage
            anchors.fill: parent
            cache: false
            source: "qrc:/windowPicture/down.png"
        }

        onClicked: {
            //console.log("OptionAugPopArea.qml downArea onClicked -->")
            fastChange.stop()
            if (mainWindow.curThreshold > mainWindow.minThreshold)
                mainWindow.curThreshold -= 1
            //senderdata.setTriggerMode(mainWindow.modelName, mainWindow.curThreshold - mainWindow.minThreshold)
            senderdata.modifyAugAlarmData(mainWindow.curThreshold - mainWindow.minThreshold)
        }
        onPressed: {
            //console.log("OptionAugPopArea.qml downArea onPressed -->")
            mainWindow.flag = false
            mainWindow.secsSinceEpoch = parseInt(new Date / 1000)
            fastChange.start()
        }
        //onReleased: fastChange.stop()
    }
}
