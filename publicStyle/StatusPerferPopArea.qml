import QtQuick 2.0

Rectangle {
    id: mainWindow

    visible: false

    radius: 12
    color: "#EDF2FF"

    readonly property int marginV: 30
    readonly property int buttonW: 230
    readonly property int buttonH: 55
    readonly property int marginH: 70

    property var btnObjectId: display
    property var popObjectId: pumpPopArea

    function btnSelectStatus(id, flag)
    {
        if (id === btnObjectId) return

//        console.log("id= " + id)
//        console.log("flag= " + flag)

        popObjectId.visible = false
        btnObjectId.curColor = btnObjectId.orgColor
        btnObjectId = id
        switch(flag)
        {
            case 1:
                displayPopArea.visible = true
                popObjectId = displayPopArea
                break
            case 2:
                audioPopArea.visible = true
                popObjectId = audioPopArea;
                break
            case 3:
                printerPopArea.visible = true
                popObjectId = printerPopArea
                break
            case 4:
                pumpPopArea.visible = true
                popObjectId = pumpPopArea
                break
        }
    }

    Rectangle {
        id: leftbackgroup
        width: mainWindow.buttonW - mainWindow.marginH + 70
        height: mainWindow.buttonH + 20

        anchors.left: parent.left
        anchors.leftMargin: display.anchors.leftMargin - 10
        anchors.top: btnObjectId.top
        anchors.topMargin: -10

        color: "#A1AAD3"
        radius: 12
    }

    DisplayPopArea {
        id: displayPopArea

        anchors.top: mainWindow.top
        anchors.topMargin: 15
        anchors.right: mainWindow.right
        anchors.rightMargin: 5
        anchors.bottom: mainWindow.bottom
        anchors.bottomMargin: 10
        anchors.left: leftbackgroup.right
        anchors.leftMargin: -13

        color: leftbackgroup.color
    }

    AudioPopArea {
        id: audioPopArea

        anchors.top: mainWindow.top
        anchors.topMargin: 15
        anchors.right: mainWindow.right
        anchors.rightMargin: 5
        anchors.bottom: mainWindow.bottom
        anchors.bottomMargin: 10
        anchors.left: leftbackgroup.right
        anchors.leftMargin: -13

        color: leftbackgroup.color
    }

    PrinterPopArea {
        id: printerPopArea

        anchors.top: mainWindow.top
        anchors.topMargin: 15
        anchors.right: mainWindow.right
        anchors.rightMargin: 5
        anchors.bottom: mainWindow.bottom
        anchors.bottomMargin: 10
        anchors.left: leftbackgroup.right
        anchors.leftMargin: -13

        color: leftbackgroup.color
    }

    PumpPopArea {
        id: pumpPopArea

        anchors.top: mainWindow.top
        anchors.topMargin: 15
        anchors.right: mainWindow.right
        anchors.rightMargin: 5
        anchors.bottom: mainWindow.bottom
        anchors.bottomMargin: 10
        anchors.left: leftbackgroup.right
        anchors.leftMargin: -13

        color: leftbackgroup.color
    }

    Component.onCompleted: {
        display.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        audio.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        printer.btnSelectStatus.connect(mainWindow.btnSelectStatus)
        pumpOption.btnSelectStatus.connect(mainWindow.btnSelectStatus)
    }

    OptionButton {
        id: display

        anchors.top: mainWindow.top
        anchors.topMargin: 40
        marginH: mainWindow.marginH
        width: mainWindow.buttonW
        height: mainWindow.buttonH
        text: "Display"
        pointSize: 16
        btnID: 1
    }

    OptionButton {
        id: audio

        anchors.top: display.bottom
        anchors.topMargin: mainWindow.marginV
        marginH: mainWindow.marginH
        width: mainWindow.buttonW
        height: mainWindow.buttonH
        text: "Audio"
        pointSize: 16
        btnID: 2
    }

    OptionButton {
        id: printer

        anchors.top: audio.bottom
        anchors.topMargin: mainWindow.marginV
        marginH: mainWindow.marginH
        width: mainWindow.buttonW
        height: mainWindow.buttonH
        text: "Printer"
        pointSize: 16
        btnID: 3
    }

    OptionButton {
        id: pumpOption

        anchors.top: printer.bottom
        anchors.topMargin: mainWindow.marginV
        marginH: mainWindow.marginH
        width: mainWindow.buttonW
        height: mainWindow.buttonH
        text: "Pump        Options"
        pointSize: 16
        btnID: 4
    }
}
