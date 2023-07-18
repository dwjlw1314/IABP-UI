import QtQuick 2.0
import QtQuick.Layouts 1.12
import QtQuick.Controls 2.5

Rectangle {
    id: mainWindow

    visible: false
    radius: 12

    Connections {
        target: publicData
        onModifyEcgGainData: {
            switch(value)
            {
            case 0:
                gainAuto.checked = true; break
            case 50:
                gain50.checked = true; break
            case 100:
                gain100.checked = true; break
            case 200:
                gain200.checked = true; break
            }
        }
    }

    Rectangle {
        id: one
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: parent.top
        anchors.leftMargin: 30
        height: parent.height*0.25

        color: "transparent"

        Text {
            text: "HF:"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.pointSize: 15
            anchors.fill: parent
        }

        RowLayout {
            anchors.left: parent.left
            anchors.leftMargin: 80
            width: 130
            height: parent.height

            RadioButton {
                id: hfstart
                checked: true
                text: qsTr("Enable")
                font.pointSize: 15
                onClicked: publicData.setEcgHeightFilter(1);

            }

            RadioButton {
                id: hfclose
                text: qsTr("Disable")
                font.pointSize: 15
                onClicked: publicData.setEcgHeightFilter(0);
            }
        }
    }

    Rectangle {
        id: two
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: one.bottom
        anchors.leftMargin: 30
        height: parent.height*0.25

        color: "transparent"

        Text {
            text: "LF:"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.pointSize: 15
            anchors.fill: parent
        }

        RowLayout {
            anchors.left: parent.left
            anchors.leftMargin: 80
            width: 130
            height: parent.height

            RadioButton {
                id: lf40
                text: qsTr("40HZ")
                font.pointSize: 15
                onClicked: publicData.setEcgLowFilter(1);
            }

            RadioButton {
                id: lfelec
                text: qsTr("Electrotome")
                font.pointSize: 15
                onClicked: publicData.setEcgLowFilter(2);
            }

            RadioButton {
                id: lfclose
                checked: true
                text: qsTr("Disable")
                font.pointSize: 15
                onClicked: publicData.setEcgLowFilter(0);
            }
        }
    }

    Rectangle {
        id: three
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: two.bottom
        anchors.leftMargin: 30
        height: parent.height*0.25

        color: "transparent"

        Text {
            text: "XB:"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.pointSize: 15
            anchors.fill: parent
        }

        RowLayout {
            anchors.left: parent.left
            anchors.leftMargin: 80
            width: 130
            height: parent.height

            RadioButton {
                id: xbstart
                text: qsTr("Enable")
                font.pointSize: 15
                onClicked: publicData.setEcgHarFilter(1);

            }

            RadioButton {
                id: xbclose
                text: qsTr("Disable")
                checked: true
                font.pointSize: 15
                onClicked: publicData.setEcgHarFilter(0);
            }
        }
    }

    Rectangle {
        id: four
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.top: three.bottom
        anchors.leftMargin: 30
        height: parent.height*0.25

        color: "transparent"

        Text {
            text: "Gain:"
            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            font.pointSize: 15
            anchors.fill: parent
        }

        RowLayout {
            anchors.left: parent.left
            anchors.leftMargin: 80
            width: 130
            height: parent.height

            RadioButton {
                id: gain50
                text: qsTr("0.5 mv")
                font.pointSize: 15
                onClicked: senderdata.setEcgGainDim(50);
            }

            RadioButton {
                id: gain100
                text: qsTr("1.0 mv")
                font.pointSize: 15
                onClicked: senderdata.setEcgGainDim(100);
            }

            RadioButton {
                id: gain200
                text: qsTr("2.0 mv")
                font.pointSize: 15
                onClicked: senderdata.setEcgGainDim(200);
            }

            RadioButton {
                id: gainAuto
                checked: true
                text: qsTr("Auto")
                font.pointSize: 15
                onClicked: senderdata.setEcgGainDim(0);
            }
        }
    }

    /* test env Delete */
//    Rectangle {
//        id: xxx
//        visible: true
//        anchors.right: parent.right
//        anchors.rightMargin: 10
//        anchors.top: parent.top
//        anchors.topMargin: 10
//        border.color: "red"
//        border.width: 2
//        width: 200
//        height: 200

//        color: "green"

//        property int up_one: 0
//        property int down_one: 0

//        Connections {
//            target: publicData
//            onSetDebugLeftValue: {
//                xxx.up_one = value
//            }
//            onSetDebugRightValue: {
//                xxx.down_one = value
//            }
//        }

//        Text {
//            id: up
//            text: xxx.up_one
//            anchors.left: xxx.left
//            anchors.top: xxx.top
//            width: xxx.width
//            height: xxx.height*0.5
//            color: "red"
//            horizontalAlignment: Text.AlignHCenter
//            verticalAlignment: Text.AlignVCenter
//            font.pointSize: 40
//        }

//        Text {
//            id: down
//            text: xxx.down_one
//            anchors.left: xxx.left
//            anchors.bottom: xxx.bottom
//            width: xxx.width
//            height: xxx.height*0.5
//            color:"black"
//            horizontalAlignment: Text.AlignHCenter
//            verticalAlignment: Text.AlignVCenter
//            font.pointSize: 40
//        }
//    }
}
