import QtQuick 2.6
import QtQuick.Window 2.2
import QtQuick.Controls 2.2
import "../controlWindow"

Window {
    id: window
    visible: true
//    width: Screen.width
//    height: Screen.height

    width: 1280
    height: 800

    color: "#000000"

    //去标题栏
    //title: qsTr("SG-1000 Control Screen")
    flags: Qt.Window | Qt.FramelessWindowHint

    //用来存储主窗口x,y坐标
    property int mainWindowX: 0
    property int mainWindowY: 0

    x: mainWindowX
    y: mainWindowY

    ControlWindow {
        id: controlWindowCom

        width: window.width
        height: window.height

        color: "black"
    }
}
