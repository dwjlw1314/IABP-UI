import QtQuick 2.0
import QtCharts 2.3
import "../publicStyle"

Item {
    id: mainWindow

    readonly property int remove_points: 20

    property int poltWidth : 990
    property string fontColor: "white"
    property string bolloonValue: "--"
    property string augAlarmValue: "100"
    property string iabFreqValue: "1:2"

    property var addSeries: series1
    property var delSeries: series2

    /* 原始值缩放100后到值, 注释取消 */
    property real axisY_min: -750
    property real axisY_max: 750

    property int plotAreaWidth: 0
    property bool enable_remove: false

    property int index: 0

    ShapeRectangle {
        id: leftArea
        width: mainWindow.poltWidth
        leftText: "Balloon Pressure"
        textColor: "white"
    }

    Text {
        id: up
        text: qsTr("Inflated")
        font.pointSize: 10
        font.bold: true
        color: "white"
        anchors.left: balloonScaleStyle.right
        anchors.leftMargin: 16
        anchors.top: leftArea.bottom
        anchors.topMargin: 3
    }

    Text {
        id: down
        text: qsTr("Deflated")
        font.pointSize: 10
        font.bold: true
        color: "white"
        anchors.left: balloonScaleStyle.right
        anchors.leftMargin: 16
        anchors.bottom: balloon_chartview.bottom
        anchors.bottomMargin: 8
    }

    ShapeRectangle {
        id: rightArea
        width: parent.width - leftArea.width
        anchors.left: leftArea.right
        anchors.leftMargin: 3
        leftText: "Augmentaion"
        rightText: "mmHg"
        textColor: fontColor
    }

    readonly property int showHeight: mainWindow.height - rightArea.height

    BalloonScaleStyle {
        id: balloonScaleStyle
        width: 40
        height: showHeight

        color: "transparent"

        anchors.left: parent.left
        anchors.top: leftArea.bottom
        anchors.topMargin: 5
    }

    Connections {
        target: publicData
        onBalloonQueueChange: {

            for(var i = 0; i < balloon_value.length; i++)
            {
                var value = balloon_value[i++];
                var start_end = balloon_value[i++]
                var draw_method = balloon_value[i++]
                mainWindow.index = balloon_value[i];

                if (0 === start_end && mainWindow.enable_remove)
                {
                    //console.log("delSeries.removePoints-> 20")
                    delSeries.removePoints(0, mainWindow.remove_points)
                }
                else
                {
                    if(mainWindow.index <= mainWindow.plotAreaWidth - mainWindow.remove_points)
                    {
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

        onBalloonMaxValue: {
            mainWindow.bolloonValue = maxValue;
        }

        onModifyTriggerRate: {
            switch(value)
            {
                case 0 : mainWindow.iabFreqValue = "1:1"; break
                case 1 : mainWindow.iabFreqValue = "1:2"; break
                case 2 : mainWindow.iabFreqValue = "1:3"; break
            }
        }

        onClearLineSeries: {
            series1.clear()
            series2.clear()
            mainWindow.enable_remove = false
            senderdata.clearFeedback(0x02)
        }

        onSetBalloonGainData: {
            axisY_min = bottom
            axisY_max = top
        }
    }

    Connections {
        target: senderdata
        onModifyAugAlarmData: {
            mainWindow.augAlarmValue = value
        }
    }

    ChartView {
        //title: "BalloonPressure wave"
        //anchors.fill: parent
        id: balloon_chartview
        legend.visible: false
        antialiasing: true
        backgroundColor: "transparent"
        anchors.left: parent.left
        anchors.top: leftArea.bottom
        anchors.topMargin: 15
        width: leftArea.width + 19
        height: parent.height - 31
        margins.left: 0
        margins.right: 0
        margins.top: 0
        margins.bottom: 0
        plotAreaColor: "transparent"

        Component.onCompleted: {
            mainWindow.plotAreaWidth = plotArea.width;
            console.log("BalloonPressure plotArea.width->" + plotArea.width)
            console.log("BalloonPressure plotArea.height->" + plotArea.height)
            publicData.setBalloonPlotAttr(plotArea.width, plotArea.height)
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
            color: "white"
            width: 2.0
            useOpenGL: true
        }

        LineSeries {
            id: series2
            axisX: axisX
            axisY: axisY
            color: "white"
            width: 2.0
            useOpenGL: true
        }
    }

    Rectangle{
        id: first
        anchors.left: rightArea.left
        anchors.top: rightArea.bottom
        width: rightArea.width - 10
        height: showHeight*0.65
        color: "transparent"

        Connections {
            target: publicData
            onModifyAugmentData: {
                //console.log(value) //0~9
                singal.showbit = 0x3ff >> value
            }
        }

        JumpGridStyle {
            id: singal
            anchors.left: first.left
            anchors.leftMargin: 15
            anchors.top: first.top
            anchors.topMargin: 10
            width: 40
            height: first.height - 12
            backcolor: "green"
            bordercolor: "transparent"
        }

        Text {
            text: bolloonValue
            color: fontColor
            font.bold: true
            font.pointSize: 56
            anchors.fill: parent
            anchors.rightMargin: 5
            horizontalAlignment: Text.AlignRight
            verticalAlignment: Text.AlignVCenter
        }
    }

    Rectangle {
        id: midLine
        height: 3;
        width: rightArea.width - 25
        anchors.top: first.bottom
        anchors.topMargin: 6
        anchors.left: rightArea.left
        anchors.leftMargin: 10
        radius: 5
        color: fontColor
    }

    Rectangle{
        id: second
        anchors.left: rightArea.left
        anchors.top: midLine.bottom
        width: rightArea.width - 10
        height: showHeight*0.35
        color: "transparent"

        Text {
            text: "Aug Alarm"
            color: fontColor
            font.pointSize: 15
            font.bold: true
            anchors.left: second.left
            anchors.leftMargin: 10
            anchors.top: second.top
        }

        Text {
            text: augAlarmValue
            color: fontColor
            font.pointSize: 15
            font.bold: true
            anchors.right: second.right
            anchors.top: second.top
        }

        Text {
            text: "IAB Frequency"
            color: fontColor
            font.pointSize: 15
            font.bold: true
            anchors.left: second.left
            anchors.leftMargin: 10
            anchors.bottom: second.bottom
            anchors.bottomMargin: 5
        }

        Text {
            text: iabFreqValue
            color: fontColor
            font.pointSize: 15
            font.bold: true
            anchors.right: second.right
            anchors.bottom: second.bottom
            anchors.bottomMargin: 5
        }
    }
}
