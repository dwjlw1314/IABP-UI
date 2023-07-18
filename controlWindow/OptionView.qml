import QtQuick 2.0
import QtQuick.Controls 2.2
import "../publicStyle"

Item {
    id: optionWindow

    property int btnMargin: 0
    property int optionBit: 0

    function popAllButton(flag)
    {
        optionBit = 0
        triggerArea.setMouseEvent(flag)
        sourceArea.setMouseEvent(flag)
        iabArea.setMouseEvent(flag)
        augmentationArea.setMouseEvent(flag)
        timingArea.setMouseEvent(flag)
    }

    function showStatusChange(id, p_opacity, p_height)
    {
        //console.log("showStatusChange p_opacity=" + p_opacity)
        if (id === triggerArea)
        {
            /* option 按钮激活触发status第二个按钮状态 */
            optionWindow.optionBit = p_opacity ? optionWindow.optionBit | 0x01 : optionWindow.optionBit & 0x1e

            if(p_opacity) sourceArea.anchors.leftMargin -= 5; else sourceArea.anchors.leftMargin += 5;
            triggerPop.visible = triggerPop.visible ? false : true
            triggerPop.opacity = p_opacity
            triggerPop.height = p_height
        }
        else if (id === sourceArea)
        {
            optionWindow.optionBit = p_opacity ? optionWindow.optionBit | 0x02 : optionWindow.optionBit & 0x1d
            if(p_opacity) iabArea.anchors.leftMargin -= 5; else iabArea.anchors.leftMargin += 5;
            sourcePop.visible = sourcePop.visible ? false : true
            sourcePop.opacity = p_opacity
            sourcePop.height = p_height
        }
        else if (id === iabArea)
        {
            optionWindow.optionBit = p_opacity ? optionWindow.optionBit | 0x04 : optionWindow.optionBit & 0x1b
            if(p_opacity) augmentationArea.anchors.leftMargin -= 5; else augmentationArea.anchors.leftMargin += 5;
            iabPop.visible = iabPop.visible ? false : true
            iabPop.opacity = p_opacity
            iabPop.height = p_height
        }
        else if (id === augmentationArea)
        {
            optionWindow.optionBit = p_opacity ? optionWindow.optionBit | 0x08 : optionWindow.optionBit & 0x17
            if(p_opacity) timingArea.anchors.leftMargin -= 5; else timingArea.anchors.leftMargin += 5;
            augmentationPop.visible = augmentationPop.visible ? false : true
            augmentationPop.opacity = p_opacity
            augmentationPop.height = p_height
        }
        else if (id === timingArea)
        {
            optionWindow.optionBit = p_opacity ? optionWindow.optionBit | 0x10 : optionWindow.optionBit & 0x0f
            //if(p_opacity) augmentationArea.anchors.leftMargin -= 5; else augmentationArea.anchors.leftMargin += 5;
            timingPop.visible = timingPop.visible ? false : true
            timingPop.opacity = p_opacity
            timingPop.height = p_height
        }
    }

    OptionRectangle {
        id: triggerArea
        anchors.left: parent.left
        anchors.leftMargin: btnMargin
        anchors.top: parent.top
        anchors.topMargin: 5

        property int marginV: 30
        property int buttonH: 50
        readonly property int marginH: 40

        btnName: "Trigger"

        Component.onCompleted: {
            triggerArea.showStatusChange.connect(optionWindow.showStatusChange)
        }

        OptionRectanglePop {
            id: triggerPop
            y: 50
            width: triggerArea.width
            radius: triggerArea.radius
            visible: false
            property var objectid: ecg
            readonly property string modelName: "trigger_mode"

            function btnSelectStatus(id, flag)
            {
                if (id === objectid) return
                objectid.curColor = objectid.orgColor
                objectid = id

                senderdata.setTriggerMode(modelName, flag)
            }

            Connections {
                target: publicData
                onSetTriggerMode: {
                    switch (value & 0x07)
                    {
                        case 0:
                            triggerPop.objectid = ecg
                            ecg.curColor = "white"
                            break
                        case 1:
                            triggerPop.objectid = pressure
                            pressure.curColor = "white"
                            break
                        case 2:
                            triggerPop.objectid = pacerV
                            pacerV.curColor = "white"
                            break
                        case 3:
                            triggerPop.objectid = pacerA
                            pacerA.curColor = "white"
                            break
                        case 4:
                            triggerPop.objectid = internal
                            internal.curColor = "white"
                            break
                    }
                }
            }

            Component.onCompleted: {
                ecg.btnSelectStatus.connect(triggerPop.btnSelectStatus)
                pressure.btnSelectStatus.connect(triggerPop.btnSelectStatus)
                pacerV.btnSelectStatus.connect(triggerPop.btnSelectStatus)
                pacerA.btnSelectStatus.connect(triggerPop.btnSelectStatus)
                internal.btnSelectStatus.connect(triggerPop.btnSelectStatus)
            }

            OptionButton {
                id: ecg
                anchors.top: triggerPop.top
                anchors.topMargin: triggerArea.marginV
                marginH: triggerArea.marginH
                width: triggerPop.width
                height: triggerArea.buttonH
                text: "ECG"
                btnID: 0
            }
            OptionButton {
                id: pressure
                anchors.top: ecg.bottom
                anchors.topMargin: triggerArea.marginV
                marginH: triggerArea.marginH
                width: triggerPop.width
                height: triggerArea.buttonH
                text: "Pressure"
                btnID: 1
            }
            OptionButton {
                id: pacerV
                anchors.top: pressure.bottom
                anchors.topMargin: triggerArea.marginV
                marginH: triggerArea.marginH
                width: triggerPop.width
                height: triggerArea.buttonH
                text: "Pacer V/AV"
                btnID: 2
            }
            OptionButton {
                id: pacerA
                anchors.top: pacerV.bottom
                anchors.topMargin: triggerArea.marginV
                marginH: triggerArea.marginH
                width: triggerPop.width
                height: triggerArea.buttonH
                text: "Pacer A"
                btnID: 3
            }
            OptionButton {
                id: internal
                anchors.top: pacerA.bottom
                anchors.topMargin: triggerArea.marginV
                marginH: triggerArea.marginH
                width: triggerPop.width
                height: triggerArea.buttonH
                text: "Internal"
                btnID: 4
            }
        }
    }

    OptionRectangle {
        id: sourceArea
        anchors.left: triggerArea.right
        anchors.leftMargin: btnMargin
        anchors.top: parent.top
        anchors.topMargin: 5
        topBtnWidth: 200
        popAreaH: 300

        property int marginV: 30
        property int buttonH: 80
        readonly property int marginH: 40

        btnName: "Sources"

        Component.onCompleted: {
            sourceArea.showStatusChange.connect(optionWindow.showStatusChange)
        }

        OptionRectanglePop {
            id: sourcePop
            visible: false
            y: sourceArea.topBtnHeight
            width: sourceArea.width
            radius: sourceArea.radius

            Connections {
                target: publicData
                onSetTriggerSource: {
                    ecgs.currentIndex = value & 0x0F
                    pressures.currentIndex = value >> 5 & 0x01
                }

                onSetThresholdData: {
                    thresholdArea.curThreshold = value
                }
            }

            OptionButtonS {
                id: ecgs
                anchors.top: sourcePop.top
                anchors.topMargin: 5
                width: sourcePop.width
                height: sourceArea.buttonH
                text: "ECG"
                totalIndex: 8
                currentIndex: 1
                totalVaule: { 0: "I", 1: "II", 2: "III", 3: "AVR", 4: "AVL", 5: "AVF", 6: "V", 7: "EXIT" }  //actual value
            }

            Rectangle {
                id: sourceMidLine
                height: 1;
                width: parent.width - 10;
                anchors.top: ecgs.bottom
                //anchors.topMargin: 3
                anchors.left: parent.left
                anchors.leftMargin: 5
                color: "darkgrey"
            }

            OptionButtonS {
                id: pressures
                anchors.top: sourceMidLine.bottom
                anchors.topMargin: 5
                width: sourcePop.width
                height: sourceArea.buttonH
                text: "Pressure"

                totalIndex: 2
                currentIndex: 0
                totalVaule: { 0: "INNER", 1: "EXIT" }  //actual value
            }

            OptionButton {
                id: thresholdsBtn
                marginH: 10
                anchors.bottom: sourcePop.bottom
                anchors.bottomMargin: 5
                width: sourcePop.width
                height: 40
                orgColor: "#859DDE"
                curColor: "#859DDE"
                text: "Thresholds"
            }

            function btnClickStatus(id)
            {
                //console.log("btnClickStatus")
                id.curColor = id.orgColor
                if (thresholdArea.threshold_visable)
                    thresholdArea.threshold_visable = false
                else
                    thresholdArea.threshold_visable = true
            }

            OptionThresholdPopArea {
                id: thresholdArea
                x: sourcePop.x + sourcePop.width / 4
                anchors.top: sourcePop.bottom
                anchors.topMargin: 5
                width: sourcePop.width / 2
                height: 140
            }

            Component.onCompleted: {
                thresholdsBtn.btnSelectStatus.connect(sourcePop.btnClickStatus)
            }
        }
    }

    OptionRectangle {
        id: iabArea
        anchors.left: sourceArea.right
        anchors.leftMargin: btnMargin
        anchors.top: parent.top
        anchors.topMargin: 5
        topBtnWidth: 200
        popAreaH: 350

        property int marginV: 30
        property int buttonH: 50
        readonly property int marginH: 40

        btnName: "IAB Frequency"

        Component.onCompleted: {
            iabArea.showStatusChange.connect(optionWindow.showStatusChange)
        }

        OptionRectanglePop {
            id: iabPop
            visible: false
            y: iabArea.topBtnHeight
            width: iabArea.width
            radius: iabArea.radius

            property var objectid: ecg
            readonly property string modelName: "trigger_rate"

            function btnSelectStatus(id, flag)
            {
                if (id === objectid) return
                objectid.curColor = objectid.orgColor
                objectid = id

                senderdata.setTriggerMode(modelName, flag)
            }

            Connections {
                target: publicData
                onSetTriggerRate: {
                    switch (value & 0x03)
                    {
                        case 0:
                            iabPop.objectid = onn_one
                            onn_one.curColor = "white"
                            break
                        case 1:
                            iabPop.objectid = onn_two
                            onn_two.curColor = "white"
                            break
                        case 2:
                            iabPop.objectid = onn_three
                            onn_three.curColor = "white"
                            break
                    }
                }
            }

            Component.onCompleted: {
                onn_one.btnSelectStatus.connect(iabPop.btnSelectStatus)
                onn_two.btnSelectStatus.connect(iabPop.btnSelectStatus)
                onn_three.btnSelectStatus.connect(iabPop.btnSelectStatus)
            }

            OptionButton {
                id: onn_one
                anchors.top: iabPop.top
                anchors.topMargin: iabArea.marginV
                marginH: iabArea.marginH
                width: iabPop.width
                height: iabArea.buttonH
                text: "1:1"
                btnID: 0
            }
            OptionButton {
                id: onn_two
                anchors.top: onn_one.bottom
                anchors.topMargin: iabArea.marginV
                marginH: iabArea.marginH
                width: iabPop.width
                height: iabArea.buttonH
                text: "1:2"
                btnID: 1
            }
            OptionButton {
                id: onn_three
                anchors.top: onn_two.bottom
                anchors.topMargin: iabArea.marginV
                marginH: iabArea.marginH
                width: iabPop.width
                height: iabArea.buttonH
                text: "1:3"
                btnID: 2
            }
        }
    }

    OptionRectangle {
        id: augmentationArea
        anchors.left: iabArea.right
        anchors.leftMargin: btnMargin
        anchors.top: parent.top
        anchors.topMargin: 5
        topBtnWidth: 210
        popAreaH: 250

        property int marginV: 30
        property int buttonH: 50
        readonly property int marginH: 40

        btnName: "Augmentation"

        Component.onCompleted: {
            augmentationArea.showStatusChange.connect(optionWindow.showStatusChange)
        }

        OptionRectanglePop {
            id: augmentationPop
            visible: false
            y: augmentationArea.topBtnHeight
            width: augmentationArea.width
            radius: augmentationArea.radius

            OptionAugPopArea {
                id: augmentationPopArea
                anchors.top: augmentationPop.top
                width: augmentationPop.width
                height: augmentationPop.height
            }
        }
    }

    OptionRectangle {
        id: timingArea
        anchors.left: augmentationArea.right
        anchors.leftMargin: btnMargin
        anchors.top: parent.top
        anchors.topMargin: 5
        topBtnWidth: 150
        popAreaH: 200

        property int marginV: 30
        property int buttonH: 50
        readonly property int marginH: 40

        btnName: "Timing"

        Component.onCompleted: {
            timingArea.showStatusChange.connect(optionWindow.showStatusChange)
        }

        OptionRectanglePop {
            id: timingPop
            visible: false
            y: timingArea.topBtnHeight
            width: timingArea.width
            radius: timingArea.radius

            OptionButtonT {
                id: inflationBtn

                anchors.top: timingPop.top
                width: timingPop.width
                height: timingPop.height/2

                text: "Inflation"
            }

            Rectangle {
                id: timingMidLine
                height: 1;
                width: parent.width - 10;
                anchors.top: inflationBtn.bottom
                anchors.left: timingPop.left
                anchors.leftMargin: 5
                color: "darkgrey"
            }

            OptionButtonT {
                id: deflationBtn

                anchors.top: timingMidLine.bottom
                width: timingPop.width
                height: timingPop.height/2

                text: "Deflation"
            }
        }
    }
}
