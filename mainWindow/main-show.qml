import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import "../showWindow"

Window {
    id: window
    visible: true
//    width: Screen.width
//    height: Screen.height

    width: 1280
    height: 800

    color: "#000000"

    //去标题栏
    //title: qsTr("VAD Show Screen")
    flags: Qt.Window | Qt.FramelessWindowHint

    //用来存储主窗口x,y坐标
    property int mainWindowX: 1280
    property int mainWindowY: 0

    x: mainWindowX
    y: mainWindowY

    //隐藏鼠标指针
    MouseArea {
        anchors.fill: parent
        cursorShape: "BlankCursor"
    }

    Connections {
        target: senderdata
        onSetShowMoreEcgWave: {
            swipeView.currentIndex = index;
        }
    }

    SwipeView {
        id: swipeView
        currentIndex: 0
        anchors.fill: parent
        orientation: Qt.Horizontal

        ShowWindow {
            id: showWindowCom

            width: window.width
            height: window.height

            color: "black"
        }

        ShowEcgWindow {
            id: showEcgWindowCom

            width: window.width
            height: window.height

            color: "black"
        }
    }
}
