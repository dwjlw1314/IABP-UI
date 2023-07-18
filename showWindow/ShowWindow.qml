import QtQuick 2.0

Rectangle {
    id: showWindow

    EcgView {
        id: ecgView
        width: parent.width
        height: 170
        poltWidth: 990
        anchors.left: parent.left
        anchors.top: parent.top
    }

    ArterialView {
        id: arterialView
        width: parent.width
        height: 250
        poltWidth: 990
        anchors.left: parent.left
        anchors.top: ecgView.bottom
    }

    BalloonPressure {
        id: balloonView
        width: parent.width
        height: 200
        poltWidth: 990
        anchors.left: parent.left
        anchors.top: arterialView.bottom
    }

    MessageIndication {
        id: msgView
        width: parent.width
        poltWidth: 990
        height: showWindow.height - ecgView.height - arterialView.height - balloonView.height
        anchors.left: parent.left
        anchors.top: balloonView.bottom
    }

    Component.onCompleted: {
        console.log("Windows geometry->" + showWindow.width + ":" + showWindow.height)
    }
}

