import QtQuick 2.0
import QtMultimedia 5.8

Item {
    id: mainWindow

    readonly property int rowNumbers: 4
    readonly property int objNumbers: 8
    readonly property int textLeftMargin: 100
    readonly property int fontSize: 14
    property var objId: [oneImage, oneInfo, twoImage, twoInfo, threeImage, threeInfo, fourImage, fourInfo]

    property int currIndex: 0

    SoundEffect {
        id: playSound
    }

    Connections {
        target: publicData
        onSendAlarmMessage: {
            //alarm_type msg

            var imagePath = ""
            switch(alarm_type)
            {
                case 0: imagePath = ""; break;
                case 1: imagePath = ""; break;
                case 2: imagePath = ""; break;
            }

            if (currIndex === objNumbers)
            {
                for(var i = 0; i < objNumbers-2; i++)
                {
                    objId[i].source = objId[i+2].source
                    i++
                    objId[i].text = objId[i+2].text
                }

                objId[currIndex-2].source = imagePath
                objId[currIndex-1].text = msg
            }
            else
            {
                objId[currIndex++].source = imagePath
                objId[currIndex++].text = msg
            }
        }

        onPlaySound: {
            if (0 === type || 1 === type)
            {
                playSound.source = "qrc:/windowPicture/phone.wav"
                playSound.play()
            }
        }
    }

    Image {
        id: oneImage
        y: 0
        anchors.left: mainWindow.left
        anchors.leftMargin: 50
        source: ""
        sourceSize.width: mainWindow.height/rowNumbers
        sourceSize.height: mainWindow.height/rowNumbers
    }

    Text {
        id: oneInfo
        text: qsTr("")
        width: mainWindow.width - oneImage.width - 300
        height: mainWindow.height/rowNumbers
        anchors.top: mainWindow.top
        x: textLeftMargin
        color: "white"
        verticalAlignment: Text.AlignVCenter
        font.pointSize: fontSize
        font.bold: true
    }

    Image {
        id: twoImage
        y: mainWindow.height/rowNumbers
        anchors.left: mainWindow.left
        anchors.leftMargin: 50
        source: ""
        sourceSize.width: oneImage.width
        sourceSize.height: oneImage.height
    }

    Text {
        id: twoInfo
        text: qsTr("")
        width: mainWindow.width - oneImage.width - 300
        height: oneInfo.height
        anchors.top: oneInfo.bottom
        x: textLeftMargin
        color: "white"
        verticalAlignment: Text.AlignVCenter
        font.pointSize: fontSize
        font.bold: true
    }

    Image {
        id: threeImage
        y: mainWindow.height/rowNumbers*2
        anchors.left: mainWindow.left
        anchors.leftMargin: 50
        source: ""
        sourceSize.width: oneImage.width
        sourceSize.height: oneImage.height
    }

    Text {
        id: threeInfo
        text: qsTr("")
        width: mainWindow.width - oneImage.width - 300
        height: oneInfo.height
        anchors.top: twoInfo.bottom
        x: textLeftMargin
        color: "white"
        verticalAlignment: Text.AlignVCenter
        font.pointSize: fontSize
        font.bold: true
    }

    Image {
        id: fourImage
        y: mainWindow.height/rowNumbers*3
        anchors.left: mainWindow.left
        anchors.leftMargin: 50
        source: ""
        sourceSize.width: oneImage.width
        sourceSize.height: oneImage.height
    }

    Text {
        id: fourInfo
        text: qsTr("")
        width: mainWindow.width - oneImage.width - 300
        height: oneInfo.height
        anchors.top: threeInfo.bottom
        x: textLeftMargin
        color: "white"
        verticalAlignment: Text.AlignVCenter
        font.pointSize: fontSize
        font.bold: true
    }
}
