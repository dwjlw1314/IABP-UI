import QtQuick 2.0
import QtQuick.Controls 2.2

Item {
    id: mainWindow
    anchors.left: parent.left
    antialiasing: true

    property string text: "ECG"
    property int marginH: 40
    property int pointSize: 14
    property string showText: ""
    property int currentIndex: 10
    property int totalIndex: 0
    property var totalVaule: {0:"test"}

    onCurrentIndexChanged: {
        textInfo.text = totalVaule[currentIndex]
        console.log("onCurrentIndexChanged")
    }

    function getValue(type)
    {
        if (currentIndex < totalIndex-1 && type)
        {
            currentIndex++
        }
        else if (currentIndex > 0 && !type)
        {
            currentIndex--
        }

        textInfo.text = totalVaule[currentIndex]
        showText = totalVaule[currentIndex]

        senderdata.setTriggerMode(text, currentIndex)
    }

    Rectangle {
        id: globalArea

        width: mainWindow.width
        height: mainWindow.height

        color: "transparent"

        Text {
            id: textArea
            text: mainWindow.text
            color:"black"
            width: parent.width
            horizontalAlignment: Text.AlignHCenter
            font.pointSize: pointSize
            height: globalArea.height/3
        }

        RoundMouseArea {
            id: leftBtn
            anchors.top: textArea.bottom
            anchors.left: parent.left
            anchors.leftMargin: 8
            width: 40
            height: 40
            color: "transparent"
            radius: 55
            border.width: 1

            Image {
                id:leftImage
                anchors.fill: parent
                cache: false
                source: "qrc:/windowPicture/left.png"
            }

            onClicked: {
                mainWindow.getValue(false)
                //console.log("leftBtn clicked")
            }
//            onPressed: {
//                console.log("leftBtn pressed")
//            }
        }

        ShowTextBox {
            id: textInfo
            anchors.left: leftBtn.right
            anchors.leftMargin: 9
            anchors.top: textArea.bottom
            anchors.topMargin: 5
            width: parent.width - leftBtn.width*2 - 35
        }

        RoundMouseArea {
            id: rightBtn
            anchors.top: textArea.bottom
            anchors.right: parent.right
            anchors.rightMargin: 8
            width: 40
            height: 40
            color: "transparent"
            radius: 55
            border.width: 1

            Image {
                id: rightImage
                anchors.fill: parent
                cache: false
                source: "qrc:/windowPicture/right.png"
            }

            onClicked: {
                mainWindow.getValue(true)
                //console.log("rightBtn clicked")
            }
            onPressed: {
                //console.log("rightBtn pressed")
            }
        }
    }
}
