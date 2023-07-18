import QtQuick 2.0
import QtCharts 2.3
import "../publicStyle"

Item {
    id: mainWindow

    readonly property int remove_points: 20

    property int poltWidth : 990
    property string fontColor: "red"
    property int moveLineY: 75
    property var arterialPreValue: ["--","--","--","--","(--)"]
    property var arterialDimValue: ["150","75","0"]

    ShapeRectangle {
        id: leftArea
        width: mainWindow.poltWidth
        leftText: "Arterial"
        midText: "Source: Fiberf-Optic"
        textColor: fontColor
    }

    EcgScaleStyle {
        id: arterialScaleStyle
        width: 40
        height: 160
        intervalValue: 38
        intervalNums: 4
        anchors.left: parent.left
        anchors.top: leftArea.bottom
        anchors.topMargin: 15
        lineColor: "red"

        Text {
            id: upDim
            text: arterialDimValue[0]
            x: if (axisY_max > 99) 2; else if (axisY_max > 9) 11; else 20;
            y: -10
            color: fontColor
            font.pointSize: 12
            font.bold: true
        }

        Text {
            id: midDim
            text: arterialDimValue[1]
            x: {
                var tmp = (axisY_max+axisY_min)/2
                if (tmp > 99) 2; else if (tmp > 9) 11; else 20;
            }
            y: 66
            color: fontColor
            font.pointSize: 12
            font.bold: true
        }

        Text {
            id: downDim
            text: arterialDimValue[2]
            x: if (axisY_min >= 9) 11; else 20;
            y: 140
            color: fontColor
            font.pointSize: 12
            font.bold: true
        }
    }

    property var addSeries: series1
    property var delSeries: series2

    property real axisY_min: 0
    property real axisY_max: 150

    property int plotAreaWidth: 0

    property int index: 0
    property bool enable_remove: false

    Connections {
        target: publicData
        onArterialQueueChange: {

            for(var i = 0; i < arterial_value.length; i++)
            {
                var value = arterial_value[i++];
                var start_end = arterial_value[i++]
                var draw_method = arterial_value[i++]
                mainWindow.index = arterial_value[i];

                if (0 === start_end && mainWindow.enable_remove)
                {
                    //console.log("delSeries.removePoints-> 20")
                    delSeries.removePoints(0, mainWindow.remove_points)
                }
                else
                {
                    if(mainWindow.index <= mainWindow.plotAreaWidth - mainWindow.remove_points)
                    {
                        //console.log("delSeries.removePoints-> 1")

                        if (draw_method && mainWindow.enable_remove && delSeries.count)
                            delSeries.remove(0)
                    }
                    else
                        delSeries.clear()
                }

                if (draw_method)
                {
                    //pre_index = mainWindow.index;
                    addSeries.append(mainWindow.index, value)
                }
                else
                {
                    //console.log("addSeries replace point")
                    addSeries.replace(mainWindow.index, addSeries.at(mainWindow.index).y, mainWindow.index, value)
                }

                //console.log("mainWindow.index->" + mainWindow.plotAreaWidth)
                if (1 === start_end)
                {
                    mainWindow.enable_remove = true

                    //old = pointer
                    if (delSeries == series2)
                    {
                        delSeries = series1
                        addSeries = series2
                    }
                    else
                    {
                        delSeries = series2
                        addSeries = series1
                    }
                }
            }
        }

        onArterialValueChange: {
             ap_up.text = ap_value[0]
             ap_low.text = ap_value[1]
             ap_avg.text = "(" + ap_value[2] + ")"
        }

        onModifyATriggerSource: {
            if (value)
                leftArea.midText = "Source: EXIT"
            else
                leftArea.midText = "Source: INNER"
        }

        onModifyInflationData: {
            slidingComLeft.slidingAsexX = value * (240-slidingComLeft.slidingAreaW)/100
            slidingComLeft.showValue = value
        }

        onModifyDeflationData: {
            slidingComRight.slidingAsexX = value * (240-slidingComLeft.slidingAreaW)/100
            slidingComRight.showValue = value
        }

        onModifyThresholdData: {
            console.log("Please Set showWindow Threshold")
            /* 设置血压阈值 7~30 */
        }

        onClearLineSeries: {
            series1.clear()
            series2.clear()
            mainWindow.enable_remove = false
            senderdata.clearFeedback(0x01)
        }

        onSetArterialGainData: {
            upDim.text = top
            midDim.text = avg
            downDim.text = bottom
            axisY_min = bottom
            axisY_max = top
        }
    }

    Connections {
        target: senderdata
        onSetMoveLineMode: {
            moveLine.visible = flag ? true : false
            moveLineText.visible = flag ? true : false
        }

        onModifyMoveLineData: {
            mainWindow.moveLineY = value  //0~100
        }
    }

    Rectangle {
        id: moveLine
        visible: false
        height: 2
        width: arterial_chartview.plotArea.width
        x: arterial_chartview.plotArea.x
        //arterial_chartview.y + arterial_chartview.plotArea.height == 25 + 150
        y: arterial_chartview.plotArea.y + 175 - moveLineY * 1.5
        radius: 5
        color: "white"
    }

    Text {
        id: moveLineText
        visible: false
        text: mainWindow.moveLineY+40
        color: "white"
        font.pointSize: 10
        x: moveLine.x - 5
        y: moveLine.y - height/2
        font.bold: true
        horizontalAlignment: Text.AlignRight
        verticalAlignment: Text.AlignVCenter
    }

    ChartView {
        //title: "ArterialView wave"
        //anchors.fill: parent
        id: arterial_chartview
        legend.visible: false
        antialiasing: true
        backgroundColor: "transparent"
        anchors.left: parent.left
        anchors.top: leftArea.bottom
        width: leftArea.width + 19
        height: parent.height - 47
        margins.left: 0
        margins.right: 0
        margins.top: 0
        margins.bottom: 0
        plotAreaColor: "transparent"

        Component.onCompleted: {
            mainWindow.plotAreaWidth = plotArea.width;
            console.log("ArterialView plotArea.width->" + plotArea.width)
            console.log("ArterialView plotArea.height->" + plotArea.height)
            publicData.setArterialPlotAttr(plotArea.width, plotArea.height)
        }

        ValueAxis {
            id: axisX
            min: 0
            max: mainWindow.plotAreaWidth
            tickCount: 21
            gridVisible: false
            labelsVisible: false
            lineVisible: false
            titleVisible: false
        }

        ValueAxis {
            id: axisY
            min: axisY_min
            max: axisY_max
            tickCount: 15
            gridVisible: false
            labelsVisible: false
            lineVisible: false
            titleVisible: false
        }

        LineSeries {
            id: series1
            axisX: axisX
            axisY: axisY
            color: "red"
            width: 2.0
            useOpenGL: true
        }

        LineSeries {
            id: series2
            axisX: axisX
            axisY: axisY
            color: "red"
            width: 2.0
            useOpenGL: true
        }
    }

    SlidingComponent {
        id: slidingComLeft
        anchors.top: arterialScaleStyle.bottom
        anchors.left: parent.left
        anchors.leftMargin: 60
        height: 80
    }

    SlidingComponent {
        id: slidingComRight
        anchors.top: slidingComLeft.top
        anchors.right: leftArea.right
        anchors.rightMargin: 270
        height: 80
        textValue: "Deflation"
    }

    ShapeRectangle {
        id: rightArea
        width: parent.width - leftArea.width
        anchors.left: leftArea.right
        anchors.leftMargin: 3
        leftText: "Pressure"
        rightText: "mmHg"
        textColor: "red"
    }

    readonly property int showHeight: mainWindow.height - rightArea.height

    Rectangle{
        id: first
        anchors.left: rightArea.left
        anchors.top: rightArea.bottom
        anchors.topMargin: -8
        width: rightArea.width - 10
        height: showHeight/4
        color: "transparent"

        Text {
            id: ap_up
            text: arterialPreValue[0]
            color: fontColor
            font.pointSize: 40
            anchors.fill: parent
            anchors.rightMargin: 5
            font.bold: true
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle{
        id: second
        anchors.left: rightArea.left
        anchors.top: first.bottom
        width: rightArea.width - 10
        height: showHeight/4/2
        color: "transparent"

        Text {
            text: "Unassisted"
            color: fontColor
            font.pointSize: 13
            font.bold: true
            anchors.right: second.right
            anchors.rightMargin: 85
            anchors.bottom: second.bottom
        }

        Text {
            id: ap_aux_up
            text: arterialPreValue[1]
            color: fontColor
            font.pointSize: 25
            anchors.fill: parent
            anchors.rightMargin: 5
            font.bold: true
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: midLine
        height: 4;
        width: rightArea.width*0.7
        anchors.top: second.bottom
        anchors.topMargin: 6
        anchors.right: rightArea.right
        radius: 5
        color: fontColor
    }

    Rectangle{
        id: three
        anchors.left: rightArea.left
        anchors.top: midLine.bottom
        anchors.topMargin: -7
        width: rightArea.width - 10
        height: showHeight/4
        color: "transparent"

        Text {
            id: ap_low
            text: arterialPreValue[3]
            color: fontColor
            font.pointSize: 40
            anchors.fill: parent
            anchors.rightMargin: 5
            font.bold: true
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle{
        id: four
        anchors.left: rightArea.left
        anchors.top: three.bottom
        width: rightArea.width - 10
        height: showHeight/4/2
        color: "transparent"

        Text {
            text: "Unassisted"
            color: fontColor
            font.pointSize: 13
            font.bold: true
            anchors.right: four.right
            anchors.rightMargin: 85
            anchors.bottom: four.bottom
        }

        Text {
            id: ap_aux_low
            text: arterialPreValue[3]
            color: fontColor
            font.pointSize: 25
            anchors.fill: parent
            anchors.rightMargin: 5
            font.bold: true
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle{
        id: five
        anchors.left: rightArea.left
        anchors.top: four.bottom
        width: rightArea.width - 10
        height: showHeight/4
        color: "transparent"

        Text {
            text: "MEAN"
            color: fontColor
            font.pointSize: 13
            font.bold: true
            anchors.left: five.left
            anchors.leftMargin: 10
            anchors.top: five.top
        }

        Text {
            id: ap_avg
            text: arterialPreValue[4]
            color: fontColor
            font.pointSize: 40
            anchors.fill: parent
            anchors.rightMargin: 5
            font.bold: true
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }

}
