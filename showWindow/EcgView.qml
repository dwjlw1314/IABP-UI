import QtQuick 2.0
import QtCharts 2.3
import "../publicStyle"

Item {
    id: mainWindow

    readonly property int remove_points: 20

    property int poltWidth : 990
    property string fontColor: "#53CC71"
    property string triggerText: "ECG"
    property string heartRateValue: "--"
    property string ecgDimValue: "2.0 mv"
    readonly property string colSeriesColor: "red"
    readonly property real colSeriesWidth: 3.0
    readonly property int colSeriesNums: 19

    property var addSeries: series1
    property var delSeries: series2

    property real axisY_min: -70
    property real axisY_max: 70

    //point x
    property int index: 0
    property int plotAreaWidth: 0
    property bool enable_remove: false

    //colSerialObj index
    property bool delEnable: false
    property int delIndex: 0
    property int addIndex: 0

    property real plotArea_y: 0
    property real plotArea_h: 0

    /* 0. series; 1. start index; 2. end index; 3. end add of the point flag; 4. del flag */
    property var colSeriesMark: [[colSeries1,0,0,false,false],[colSeries2,0,0,false,false],
        [colSeries3,0,0,false,false],[colSeries4,0,0,false,false],[colSeries5,0,0,false,false],
        [colSeries6,0,0,false,false],[colSeries7,0,0,false,false],[colSeries8,0,0,false,false],
        [colSeries9,0,0,false,false],[colSeries10,0,0,false,false],[colSeries11,0,0,false,false],
        [colSeries12,0,0,false,false],[colSeries13,0,0,false,false],[colSeries14,0,0,false,false],
        [colSeries15,0,0,false,false],[colSeries16,0,0,false,false],[colSeries17,0,0,false,false],
        [colSeries18,0,0,false,false],[colSeries19,0,0,false,false],[colSeries20,0,0,false,false]]

    /* 可以把部分逻辑提到c++中，提高界面处理速度 */
    Connections {
        target: publicData
        onEcgQueueChange: {

            for(var i = 0; i < ecg_value.length; i++)
            {
                var trigger_state = ecg_value[i++]
                var value = ecg_value[i++]
                var start_end = ecg_value[i++]
                var draw_method = ecg_value[i++]
                mainWindow.index = ecg_value[i]

                if (0 === start_end && enable_remove)
                {
                    //console.log("delSeries.removePoints-> 20")
                    delSeries.removePoints(0, mainWindow.remove_points)

                    while (colSeriesMark[delIndex][4] && colSeriesMark[delIndex][1] < mainWindow.remove_points)
                    {
                        var num = 0
                        if (colSeriesMark[delIndex][2] < mainWindow.remove_points)
                        {
                            num = colSeriesMark[delIndex][0].count
                            colSeriesMark[delIndex][0].removePoints(0, num)
                            colSeriesMark[delIndex][4] = false
                            colSeriesMark[delIndex][1] = mainWindow.remove_points
                            //console.log("All del index->" + delIndex + " delnum ->" + num)
                            if (++delIndex > colSeriesNums)
                                delIndex = 0
                        }
                        else
                        {
                            num = mainWindow.remove_points - colSeriesMark[delIndex][1]
                            if (num > colSeriesMark[delIndex][0].count)
                                num = colSeriesMark[delIndex][0].count
                            colSeriesMark[delIndex][0].removePoints(0, num)
                            colSeriesMark[delIndex][1] = mainWindow.remove_points
                            //console.log("Part del index->" + delIndex + " delnum ->" + num + " colSeriesMark[delIndex][1]->" + colSeriesMark[delIndex][1])
                        }
                    }
                }
                else
                {
                    if(mainWindow.index <= mainWindow.plotAreaWidth - mainWindow.remove_points)
                    {
                        if (draw_method && enable_remove && delSeries.at(0).x)
                        {
                            delSeries.remove(0)

                            var cur_index = mainWindow.index+19
                            if (mainWindow.delEnable && cur_index >= colSeriesMark[delIndex][1])
                            {
                                //console.log(delIndex + ": removePoints->" + colSeriesMark[delIndex][1])

                                //console.log("Del index->" + delIndex + " start index->" + colSeriesMark[delIndex][1])
                                if (colSeriesMark[delIndex][0].count && mainWindow.index < colSeriesMark[delIndex][1])
                                    colSeriesMark[delIndex][0].remove(0)

                                if (colSeriesMark[delIndex][0].count)
                                {
                                    colSeriesMark[delIndex][1] = colSeriesMark[delIndex][0].at(0).x
                                }
                                else
                                {
                                    colSeriesMark[delIndex][4] = false

                                    if (++delIndex > colSeriesNums)
                                        delIndex = 0
                                }
                            }
                        }
                    }
                    else
                    {
                        delSeries.clear()

                        if (delEnable && colSeriesMark[delIndex][2] > mainWindow.plotAreaWidth - mainWindow.remove_points)
                        {
                            //console.log("clean->" + delIndex)

                            mainWindow.delEnable = false
                            colSeriesMark[delIndex][0].clear()
                            colSeriesMark[delIndex][4] = false
                            if (++delIndex > colSeriesNums)
                                delIndex = 0
                        }
                    }
                }

                if (trigger_state === 1)   //trigger start
                {
                    if (!colSeriesMark[addIndex][3])
                    {
                        //console.log("index->" + addIndex + " start index->" + mainWindow.index)

                        colSeriesMark[addIndex][1] = mainWindow.index
                        colSeriesMark[addIndex][3] = true
                    }
                }

                if (draw_method)
                {
                    addSeries.append(mainWindow.index, value)
                }
                else
                {
                    addSeries.replace(mainWindow.index, addSeries.at(mainWindow.index).y, mainWindow.index, value)
                }

                if (colSeriesMark[addIndex][3])
                {
                    if (draw_method || trigger_state === 1)
                    {
                        //add_pre_index = mainWindow.index
                        //console.log(addIndex + ": addPoints->" + mainWindow.index)
                        //console.log("x" + mainWindow.index)
                        //console.log("add index->" + addIndex + " add index ->" + mainWindow.index)

                        colSeriesMark[addIndex][0].append(mainWindow.index, value-4)
                    }
                    else
                    {
                        colSeriesMark[addIndex][0].replace(mainWindow.index,
                            colSeriesMark[addIndex][0].at(mainWindow.index).y, mainWindow.index, value-4)
                    }
                }

                if (trigger_state === 2)
                {
                    if (colSeriesMark[addIndex][3])
                    {
                        //console.log("index->" + addIndex + " total points->" + colSeriesMark[addIndex][0].count)
                        colSeriesMark[addIndex][4] = true
                        colSeriesMark[addIndex][3] = false
                        colSeriesMark[addIndex][2] = mainWindow.index
                        if (++addIndex > colSeriesNums)
                            addIndex = 0
                    }
                }

                //console.log("mainWindow.index->" + mainWindow.plotAreaWidth)
                if (start_end === 1)
                {
                    mainWindow.delEnable = true
                    enable_remove = true

                    //最后一个点强制触发结束
                    if (colSeriesMark[addIndex][3])
                    {
                        //console.log("index->" + addIndex + " end index->" + mainWindow.index)

                        colSeriesMark[addIndex][3] = false
                        colSeriesMark[addIndex][4] = true
                        colSeriesMark[addIndex][2] = mainWindow.index
                        if (++addIndex > colSeriesNums)
                            addIndex = 0

                        //console.log("index->" + addIndex + " start index->" + 0)
                        colSeriesMark[addIndex][1] = 0
                        colSeriesMark[addIndex][3] = true
                    }

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

        onEcgHeartBeatChange: {
            heartBeatTimer.start()
            heartBeat.visible = true
            heartRate.text = heart_rate
        }

        onModifyTriggerMode: {
            switch(value)
            {
                case 0: mainWindow.triggerText = "ECG"; break;
                case 1: mainWindow.triggerText = "PRESSURE"; break;
                case 2: mainWindow.triggerText = "PACER V/AV"; break;
                case 3: mainWindow.triggerText = "PACER A"; break;
                case 4: mainWindow.triggerText = "INTERNAL"; break;
            }
        }

        onModifyETriggerSource: {
            switch(value)
            {
                case 0: leftArea.midText = "Lead: Auto I"; break;
                case 1: leftArea.midText = "Lead: Auto II"; break;
                case 2: leftArea.midText = "Lead: Auto III"; break;
                case 3: leftArea.midText = "Lead: Auto AVR"; break;
                case 4: leftArea.midText = "Lead: Auto AVL"; break;
                case 5: leftArea.midText = "Lead: Auto AVF"; break;
                case 6: leftArea.midText = "Lead: Auto V"; break;
                case 7: leftArea.midText = "Lead: Auto EXIT"; break;
            }
        }

        onSetEcgGainData: {
            mainWindow.ecgDimValue = value;

            if (gain === 200)
            {
                ecgScaleStyle.height = mainWindow.plotArea_h
                ecgScaleStyle.intervalValue = ecgScaleStyle.height
                ecgScaleStyle.anchors.topMargin = mainWindow.plotArea_y
            }
            else if (gain === 100)
            {
                ecgScaleStyle.height = mainWindow.plotArea_h*0.5
                ecgScaleStyle.intervalValue = ecgScaleStyle.height
                ecgScaleStyle.anchors.topMargin = mainWindow.plotArea_y + mainWindow.plotArea_h*0.25
            }
            else if (gain === 50)
            {
                ecgScaleStyle.height = mainWindow.plotArea_h*0.25
                ecgScaleStyle.intervalValue = ecgScaleStyle.height
                ecgScaleStyle.anchors.topMargin = mainWindow.plotArea_y + mainWindow.plotArea_h*0.37
            }

            console.log("EcgGain->" + mainWindow.plotArea_h + "  " + mainWindow.plotArea_y)
        }

        onClearLineSeries: {
            for(var i = 0; i <= colSeriesNums; i++)
            {
                colSeriesMark[i][0].clear()
                colSeriesMark[i][1] = 0
                colSeriesMark[i][2] = 0
                colSeriesMark[i][3] = false
                colSeriesMark[i][4] = false
            }
            addSeries.clear()
            delSeries.clear()
            delEnable = false
            delIndex = 0
            addIndex = 0
            enable_remove = false
            senderdata.clearFeedback(0x04)
        }
    }

//    Component.onCompleted: {
//        triggerArea.showStatusChange.connect(optionWindow.showStatusChange)
//    }

    ShapeRectangle {
        id: leftArea
        width: mainWindow.poltWidth
        leftText: "ECG"
        midText: "Lead: Auto II"
        textColor: "#53CC71"
    }

    EcgScaleStyle {
        id: ecgScaleStyle
        width: 40
        height: ecg_chartview.plotArea.height  //107
        intervalValue: ecg_chartview.plotArea.height
        anchors.left: parent.left
        anchors.top: ecg_chartview.top
        anchors.topMargin: ecg_chartview.plotArea.y  //18.5
        lineColor: "green"

        Text {
            id: ecgDimension
            text: ecgDimValue
            x: 6
            width: 25
            height: parent.height
            verticalAlignment: Qt.AlignVCenter
            lineHeight: 0.8
            color: fontColor
            font.pointSize: 12
            font.bold: true
            wrapMode: Text.WordWrap
        }
    }

    ChartView {
        //title: "ECG Wave"
        //anchors.fill: parent
        id: ecg_chartview
        legend.visible: false
        antialiasing: true
        backgroundColor: "transparent"
        anchors.left: parent.left
        anchors.top: leftArea.bottom
        width: leftArea.width + 19
        height: parent.height - 11
        margins.left: 0
        margins.right: 0
        margins.top: 0
        margins.bottom: 0
        plotAreaColor: "transparent"

        Component.onCompleted: {
            mainWindow.plotAreaWidth = plotArea.width;
            console.log("plotArea.width->" + plotArea.width)
            console.log("plotArea.height->" + plotArea.height)
            plotArea_y = ecg_chartview.plotArea.y + 9.0
            plotArea_h = ecg_chartview.plotArea.height - 32.5
            publicData.setEcgPlotAttr(plotArea.width, plotArea.height);
            publicData.initGainEcg(false);
        }

        ValueAxis {
            id: axisX
            min: 0
            max: mainWindow.plotAreaWidth
            tickCount: 21
            gridVisible: false
            labelsVisible: false
            lineVisible: false  //突出的小段线不可见
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

        // 后面可以修改为动态添加
        LineSeries {
            id: series1
            axisX: axisX
            axisY: axisY
            color: "green"
            width: 2.0
            useOpenGL: true
        }

        LineSeries {
            id: series2
            axisX: axisX
            axisY: axisY
            color: "green"
            width: 2.0
            useOpenGL: true
        }

        LineSeries {
            id: colSeries1
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries2
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries3
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries4
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries5
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries6
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries7
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries8
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries9
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries10
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries11
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries12
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries13
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries14
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries15
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries16
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries17
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries18
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries19
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: colSeries20
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
    }

    ShapeRectangle {
        id: rightArea
        width: parent.width - leftArea.width
        anchors.left: leftArea.right
        anchors.leftMargin: 3
        leftText: "Rate"
        rightText: "bpm"
        textColor: "#53CC71"
    }

    Image {
        id: heartBeat
        anchors.left: rightArea.left
        anchors.leftMargin: 13
        anchors.top: rightArea.bottom
        anchors.topMargin: 20
        source: "qrc:/windowPicture/heartbeat.png"
        width: 30
        height: 30
        visible: false
        //fillMode:Image.Stretch
    }

    Timer {
        id: heartBeatTimer
        interval: 130   //设置定时器定时时间为500ms,默认1000ms
        running: false  //是否开启定时，默认是false，当为true的时候，进入此界面就开始定时
        repeat: false   //是否重复定时,默认为false
        onTriggered: heartBeat.visible = false
    }

    Text {
        id: labelText
        text: "Trigger: "
        anchors.left: rightArea.left
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        color: fontColor
        font.pointSize: 12
        font.bold: true
    }

    Text {
        text: triggerText
        anchors.left: labelText.right
        anchors.leftMargin: 5
        anchors.bottom: parent.bottom
        anchors.bottomMargin: 10
        color: fontColor
        font.pointSize: 12
        font.bold: true
    }

    Text {
        id: heartRate
        text: heartRateValue
//        width: 50
//        height: 100
        x: parent.width - heartRate.width - 10
        y: (mainWindow.height - heartRate.height)/2
        color: fontColor
        font.pointSize: 56
        font.bold: true
        horizontalAlignment: Text.AlignHCenter
        verticalAlignment: Text.AlignVCenter
    }

}
