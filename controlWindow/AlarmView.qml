import QtQuick 2.12
import "../publicStyle"

Item {
    id: alarmWindow

    property var objectId
    readonly property int parent_radius: 8
    readonly property int child_radius: 5

    signal alarmBoxChange(var id, var flag)

    function btnSelectStatus(id, flag)
    {
        exit.curColor = "#F8E04F"

        if (detailArea.visible)
            detailArea.visible = false
        else
            alarmBoxChange(objectId, false)
    }

    Rectangle {
        width: parent.width
        height: parent.height
        anchors.fill: alarmWindow
        color: "#F8E04F"
        radius: alarmWindow.parent_radius

        Rectangle {
            id: titleMsg

            property string msg: "Messages Posted"

            height: 30
            color: "black"
            anchors.top: parent.top
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.margins: 3

            radius: alarmWindow.child_radius

            Text {
                text: qsTr(titleMsg.msg)
                anchors.fill: parent
                anchors.leftMargin: 5
                verticalAlignment: Text.AlignVCenter
                color:"white"
                font.bold: true
                font.pointSize: 13
            }
        }

        Rectangle {
            id: leftArea

            width: parent.width * 0.8
            anchors.left: parent.left
            anchors.top: titleMsg.bottom
            anchors.bottom: parent.bottom

            color: "transparent"

            property string up_msg: "Autofill Failure - No Helium"
            property string mid_msg: "Unable to Update Timing"
            property string down_msg: "No Backup Vattery Detected"

            Rectangle {
                id: up
                height: leftArea.height*0.25
                color: "black"
                anchors.top: parent.top
                anchors.topMargin: leftArea.height*0.06
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 25

                border.width: 3
                border.color: "#665B25"
                radius: alarmWindow.parent_radius

                /* 白色底覆盖一层黑色背景,实现文本框顶部样式 */
//                Rectangle {
//                    anchors.top: parent.top
//                    anchors.topMargin: 3
//                    anchors.left: parent.left
//                    anchors.leftMargin: -2
//                    height: parent.height
//                    width: parent.width + 4
//                    color: "transparent"
//                    border.color: "white"
//                    border.width: 1
//                    radius: 10
//                }

                Text {
                    text: qsTr(leftArea.up_msg)
                    anchors.fill: parent
                    anchors.leftMargin: 15
                    verticalAlignment: Text.AlignVCenter
                    color:"white"
                    font.bold: true
                    font.pointSize: 15
                }
                MouseArea
                {
                    anchors.fill: parent;
                    hoverEnabled: false;
                    onClicked:
                    {
                        detailArea.visible = true
                    }
                }
            }

            Rectangle {
                id: mid
                height: leftArea.height*0.25
                color: "black"
                anchors.top: up.bottom
                anchors.topMargin: leftArea.height*0.06
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 25

                border.width: 3
                border.color: "#665B25"
                radius: alarmWindow.parent_radius

                Text {
                    text: qsTr(leftArea.mid_msg)
                    anchors.fill: parent
                    anchors.leftMargin: 15
                    verticalAlignment: Text.AlignVCenter
                    color:"white"
                    font.bold: true
                    font.pointSize: 15
                }
            }

            Rectangle {
                id: down
                height: leftArea.height*0.25
                color: "black"
                anchors.top: mid.bottom
                anchors.topMargin: leftArea.height*0.06
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.leftMargin: 25

                border.width: 3
                border.color: "#665B25"
                radius: alarmWindow.parent_radius

                Text {
                    text: qsTr(leftArea.down_msg)
                    anchors.fill: parent
                    anchors.leftMargin: 15
                    verticalAlignment: Text.AlignVCenter
                    color:"white"
                    font.bold: true
                    font.pointSize: 15
                }
            }
        }

        Rectangle {
            id: detailArea

            width: parent.width * 0.8 + 3
            anchors.left: parent.left
            anchors.top: titleMsg.bottom
            anchors.bottom: parent.bottom

            color: "#F8E04F"
            visible: false
            radius: alarmWindow.parent_radius

            Rectangle {
                id: detailTitle

                property string titleInfo: "AC is the only power source available to the IABP"

                height: 30
                color: "#505E75"
                anchors.top: parent.top
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.margins: 5

                radius: alarmWindow.child_radius

                Text {
                    text: qsTr(detailTitle.titleInfo)
                    anchors.fill: parent
                    anchors.leftMargin: 5
                    verticalAlignment: Text.AlignVCenter
                    color:"white"
                    font.bold: true
                    font.pointSize: 13
                    font.italic: true
                }
            }

            Rectangle {
                id: detailInfo

                property string bodyInfo: "1. safety"

                color: "transparent"
                anchors.top: detailTitle.bottom
                anchors.left: parent.left
                anchors.right: parent.right
                anchors.bottom: parent.bottom
                anchors.margins: 20

                Text {
                    text: qsTr(detailInfo.bodyInfo)
                    anchors.fill: parent
                    color:"black"
                    font.bold: true
                    font.pointSize: 10
                }
            }
        }

        Rectangle {
            id: rightArea

            width: parent.width * 0.2
            anchors.left: leftArea.right
            anchors.top: titleMsg.bottom
            anchors.bottom: parent.bottom

            color: "transparent"

            RoundMouseArea {
                id: upPage

                anchors.top: rightArea.top
                anchors.topMargin: upPage.height*0.35
                anchors.left: rightArea.left
                anchors.leftMargin: (rightArea.width-upPage.width)/2

                width: rightArea.width*0.4
                height: rightArea.width*0.4
                color: "transparent"
                radius: 55
                //border.width: 2

                Image {
                    id: upImage
                    anchors.fill: parent
                    cache: false
                    source: "qrc:/windowPicture/up.png"
                }

                onClicked: {
                    ;
                }
            }

            Rectangle {
                id: page_nums

                anchors.left: rightArea.left
                anchors.top: upPage.bottom
                width: rightArea.width
                height: 30

                property string page_text: "0 of 0"
                color: "transparent"

                Text {
                    text: qsTr(page_nums.page_text)
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    color:"black"
                    font.pointSize: 14
                }
            }

            RoundMouseArea {
                id: downPage

                anchors.top: page_nums.bottom
                anchors.left: rightArea.left
                anchors.leftMargin: (rightArea.width-upPage.width)/2

                width: rightArea.width*0.4
                height: rightArea.width*0.4
                color: "transparent"
                radius: 55
                //border.width: 2

                Image {
                    id: downImage
                    anchors.fill: parent
                    cache: false
                    source: "qrc:/windowPicture/down.png"
                }

                onClicked: {
                    ;
                }
            }

            Component.onCompleted: {
                exit.btnSelectStatus.connect(alarmWindow.btnSelectStatus)
            }

            OptionButton {
                id: exit

                anchors.bottom: rightArea.bottom
                anchors.bottomMargin: 10
                anchors.left: rightArea.left
                anchors.leftMargin: (rightArea.width-exit.width)*0.5

                width: rightArea.width*0.7
                height: rightArea.width*0.3

                marginH: 0
                text: "Exit"
                curColor: "#F8E04F"
                borderColor: "#726529"
            }
        }
    }
}
