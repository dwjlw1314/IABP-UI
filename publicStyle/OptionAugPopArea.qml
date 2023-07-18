import QtQuick 2.0

Item {
    id: mainWindow
    anchors.left: parent.left

    property int pointSize: 14
    readonly property string uptext: "Max"
    readonly property string downtext: "Off"
    property string url: "qrc:/windowPicture/singal.png"

    /* 根据实际值设置 */
    property int maxVaue: 10

    Rectangle {
        id: leftArea

        width: parent.width*0.6
        height: parent.height

        color: "transparent"

        Text {
            id: upText
            text: mainWindow.uptext
            anchors.top: leftArea.top
            anchors.topMargin: 5
            color:"black"
            width: parent.width
            height: 20
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: pointSize
        }

        Rectangle {
            anchors.left: leftArea.left
            anchors.leftMargin: leftArea.width / 4
            anchors.right: leftArea.right
            anchors.rightMargin: leftArea.width / 4
            anchors.top: upText.bottom
            anchors.topMargin: 13
            anchors.bottom: leftArea.bottom
            anchors.bottomMargin: 33
            width: leftArea.width / 2

            color: "#7381B0"

            JumpGridStyle {
                id: singal
                anchors.fill: parent
                anchors.margins: 5
                winterval: 4
                hinterval: 6
                backcolor: "green"
                bordercolor: "transparent"
            }
        }

        Text {
            id: downText
            text: mainWindow.downtext
            anchors.bottom: leftArea.bottom
            anchors.bottomMargin: 13
            color:"black"
            width: leftArea.width
            height: 20
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: pointSize
        }
    }

    Rectangle {
        id: rightArea

        width: parent.width*0.4
        height: parent.height
        anchors.left: leftArea.right

        color: "transparent"

        property int currentIndex: 0
        property int totalIndex: 8
        readonly property string modelName: "aug_data"

        Connections {
            target: publicData
            onSetAugmentData: {
                rightArea.currentIndex = value //0~9
                singal.showbit = 0x3ff >> value
            }
        }

        RoundMouseArea {
            id: upArea

            anchors.left: rightArea.left
            anchors.leftMargin: 10
            anchors.top: rightArea.top
            anchors.topMargin: rightArea.height/7

            width: rightArea.height/4
            height: rightArea.height/4
            color: "transparent"
            radius: 55
            border.width: 2

            onClicked: {
                //console.log("OptionAugPopArea.qml upArea onClicked -->")
                if (rightArea.currentIndex > 0)
                {
                    rightArea.currentIndex--
                    singal.showbit = singal.showbit << 1 | 0x01
                    senderdata.setTriggerMode(rightArea.modelName, singal.showbit)
                }
            }
            onPressed: {
                //console.log("OptionAugPopArea.qml upArea onPressed -->")
            }

//            Image {
//                id: upImage
//                anchors.fill: parent
//                cache: false
//                source: "qrc:/windowPicture/upPopButton.png"
//            }

        }

        RoundMouseArea {
            id: downArea

            anchors.left: rightArea.left
            anchors.leftMargin: upArea.anchors.leftMargin
            anchors.bottom: rightArea.bottom
            anchors.bottomMargin: upArea.anchors.topMargin

            width: upArea.width
            height: upArea.height
            color: "transparent"
            radius: 55
            border.width: upArea.border.width

            onClicked: {
                //console.log("OptionAugPopArea.qml downArea onClicked -->")
                if (rightArea.currentIndex <= rightArea.totalIndex)
                    rightArea.currentIndex++
                singal.showbit = 0x3ff >> rightArea.currentIndex
                senderdata.setTriggerMode(rightArea.modelName, singal.showbit)
            }
            onPressed: {
                //console.log("OptionAugPopArea.qml downArea onPressed -->")
            }

//            Image {
//                id: downImage
//                cache: false
//                source: "qrc:/windowPicture/upPopButton.png"
//            }
        }
    }
}
