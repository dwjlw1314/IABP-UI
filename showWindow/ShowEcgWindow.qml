import QtQuick 2.0
import QtCharts 2.3

/** 临时版本 删除功能 取消setShowMoreEcgWave信号绑定 */

Rectangle {
    id: mainWindow

    readonly property int remove_points: 20

    property string fontColor: "#53CC71"
    property string ecgDimValue: "2.0 mv"
    readonly property string colSeriesColor: "red"
    readonly property real colSeriesWidth: 2.0
    readonly property int colSeriesNums: 7

    property var addSeries: [series1,series2,series3,series4,series5,series6,series7]
    property var delSeries: [series11,series22,series33,series44,series55,series66,series77]

    property real axisY_min: 0
    property real axisY_max: 0
    //point x
    property int index: 0
    property int plotAreaWidth: 0
    property bool enable_remove: false

    property real plotArea_y: 0
    property real plotArea_h: 0

    Connections {
        target: publicData
        onEcgAllQueueChange: {

            for(var i = 0; i < ecg_value.length; i++)
            {
                var value = [0,0,0,0,0]
                value[0] = ecg_value[i++]
                value[1] = ecg_value[i++]
                value[2] = ecg_value[i++]
                value[3] = ecg_value[i++]
                value[4] = ecg_value[i++]
                value[5] = ecg_value[i++]
                value[6] = ecg_value[i++]
                var start_end = ecg_value[i++]
                var draw_method = ecg_value[i++]
                mainWindow.index = ecg_value[i]

                if (0 === start_end && mainWindow.enable_remove)
                {
                    //console.log("delSeries.removePoints-> 20")
                    for(var j = 0 ; j < colSeriesNums; j++)
                        delSeries[j].removePoints(0, mainWindow.remove_points)
                }
                else
                {
                    if(mainWindow.index <= mainWindow.plotAreaWidth - mainWindow.remove_points)
                    {
                        //console.log("delSeries.removePoints-> 1")

                        for(var p = 0 ; p < colSeriesNums; p++)
                        {
                            if (draw_method && mainWindow.enable_remove && delSeries[p].count)
                                delSeries[p].remove(0)
                        }
                    }
                    else
                    {
                        for(var k = 0 ; k < colSeriesNums; k++)
                            delSeries[k].clear()
                    }
                }

                if (draw_method)
                {
                    //pre_index = mainWindow.index;
                    for(var n = 0 ; n < colSeriesNums; n++)
                        addSeries[n].append(mainWindow.index, value[n])
                }
                else
                {
                    //console.log("addSeries replace point")
                    for(var m = 0 ; m < colSeriesNums; m++)
                        addSeries[m].replace(mainWindow.index, addSeries[m].at(mainWindow.index).y, mainWindow.index, value[m])
                }

                //console.log("mainWindow.index->" + mainWindow.plotAreaWidth)
                if (1 === start_end)
                {
                    mainWindow.enable_remove = true

                    //old = pointer
                    if (delSeries[0] === series11)
                    {
                        delSeries = [series1,series2,series3,series4,series5,series6,series7]
                        addSeries = [series11,series22,series33,series44,series55,series66,series77]
                    }
                    else
                    {
                        delSeries = [series11,series22,series33,series44,series55,series66,series77]
                        addSeries = [series1,series2,series3,series4,series5,series6,series7]
                    }
                }
            }
        }

        onClearLineSeries: {
            for(var i = 0; i < colSeriesNums; i++)
            {
                addSeries[i].clear()
                delSeries[i].clear()
            }
            enable_remove = false
            //senderdata.clearFeedback(0x08)
        }
    }

    Text {
        id: ecg_i
        text: "ECG I"
        x: 25
        y: 60
        width: 25
        height: 25
        color: fontColor
        font.pointSize: 12
        font.bold: true
    }

    Text {
        id: ecg_ii
        text: "ECG II"
        x: 20
        y: 165  //60+ (parent.height / plotArea.height) * 100
        width: 25
        height: 25
        color: fontColor
        font.pointSize: 12
        font.bold: true
    }

    Text {
        id: ecg_iii
        text: "ECG III"
        x: 15
        y: 273
        width: 25
        height: 25
        color: fontColor
        font.pointSize: 12
        font.bold: true
    }

    Text {
        id: ecg_VI
        text: "ECG VI"
        x: 20
        y: 380
        width: 25
        height: 25
        color: fontColor
        font.pointSize: 12
        font.bold: true
    }

    Text {
        id: ecg_AVF
        text: "ECG AVF"
        x: 5
        y: 485
        width: 25
        height: 25
        color: fontColor
        font.pointSize: 12
        font.bold: true
    }

    Text {
        id: ecg_AVL
        text: "ECG AVL"
        x: 5
        y: 595
        width: 25
        height: 25
        color: fontColor
        font.pointSize: 12
        font.bold: true
    }

    Text {
        id: ecg_AVR
        text: "ECG AVR"
        x: 5
        y: 700
        width: 25
        height: 25
        color: fontColor
        font.pointSize: 12
        font.bold: true
    }

    ChartView {
        id: ecg_chartview
        legend.visible: false
        antialiasing: true
        backgroundColor: "transparent"
        anchors.left: parent.left
        anchors.top: parent.top
        anchors.leftMargin: 30
        width: parent.width - 50   //1179  1160
        height: parent.height   //719   700
        margins.left: 0
        margins.right: 0
        margins.top: 0
        margins.bottom: 0
        plotAreaColor: "transparent"

        Component.onCompleted: {
            mainWindow.plotAreaWidth = plotArea.width;
            mainWindow.axisY_min = 0 - plotArea.height/2
            mainWindow.axisY_max = plotArea.height/2
            //console.log("xxx" + plotArea.y + "hei->" + plotArea.height)
            publicData.setAllEcgPlotAttr(plotArea.width, plotArea.height);
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

        // 后面可以修改为动态添加
        LineSeries {
            id: series1
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }

        LineSeries {
            id: series11
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }

        LineSeries {
            id: series2
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series22
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series3
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series33
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series4
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series44
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series5
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series55
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series6
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series66
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series7
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
        LineSeries {
            id: series77
            axisX: axisX
            axisY: axisY
            color: colSeriesColor
            width: colSeriesWidth
            useOpenGL: true
        }
    }
}
