import QtQuick 2.0

Item {
    id: mainWindow

    property int winterval: 3
    property int hinterval: 4
    property int showbit: 128
    property int unitwidth: 7
    property int unitradius: 5
    property string backcolor: "white"
    property string bordercolor: "white"

    onShowbitChanged: {
        one.bgColor = showbit & 0x200 ? backcolor : "white"
        two.bgColor = showbit & 0x100 ? backcolor : "white"
        three.bgColor = showbit & 0x80 ? backcolor : "white"
        four.bgColor = showbit & 0x40 ? backcolor : "white"
        five.bgColor = showbit & 0x20 ? backcolor : "white"
        six.bgColor = showbit & 0x10 ? backcolor : "white"
        seven.bgColor = showbit & 0x08 ? backcolor : "white"
        eight.bgColor = showbit & 0x04 ? backcolor : "white"
        nine.bgColor = showbit & 0x02 ? backcolor : "white"
        ten.bgColor = showbit & 0x01 ? backcolor : "white"
    }

    RoundRectangle
    {
        id: one
        width: mainWindow.width - winterval * 0
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: mainWindow.top
    }

    RoundRectangle
    {
        id: two
        width: mainWindow.width - winterval * 1
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: one.bottom
        anchors.topMargin: hinterval
    }

    RoundRectangle
    {
        id: three
        width: mainWindow.width - winterval * 2
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: two.bottom
        anchors.topMargin: hinterval
    }

    RoundRectangle
    {
        id: four
        width: mainWindow.width - winterval * 3
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: three.bottom
        anchors.topMargin: hinterval
    }

    RoundRectangle
    {
        id: five
        width: mainWindow.width - winterval * 4
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: four.bottom
        anchors.topMargin: hinterval
    }

    RoundRectangle
    {
        id: six
        width: mainWindow.width - winterval * 5
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: five.bottom
        anchors.topMargin: hinterval
    }

    RoundRectangle
    {
        id: seven
        width: mainWindow.width - winterval * 6
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: six.bottom
        anchors.topMargin: hinterval
    }

    RoundRectangle
    {
        id: eight
        width: mainWindow.width - winterval * 7
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: seven.bottom
        anchors.topMargin: hinterval
    }

    RoundRectangle
    {
        id: nine
        width: mainWindow.width - winterval * 8
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: eight.bottom
        anchors.topMargin: hinterval
    }

    RoundRectangle
    {
        id: ten
        width: mainWindow.width - winterval * 9
        height: unitwidth
        radius: unitradius
        bgColor: backcolor
        border.color: bordercolor
        anchors.left: mainWindow.left
        anchors.top: nine.bottom
        anchors.topMargin: hinterval
    }
}
