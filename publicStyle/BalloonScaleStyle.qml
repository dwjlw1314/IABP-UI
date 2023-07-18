import QtQuick 2.0
import QtQuick.Controls 1.2
import QtQuick.Controls.Styles 1.2

Rectangle {
    id: mainWindow

    //property alias value: progressBar.value
    property real realvalue: 0.0

    Connections {
        target: publicData
        onBalloonStatusChange : {
            realvalue = progressBarValue;
            progressBarAnimation.start();
        }
    }

    ProgressBar {
        id: progressBar
        x: 21
        y: 17
        width: 16
        height: mainWindow.height - 34
        orientation: Qt.Vertical
        //value: mainWindow.value
        //implicitWidth: 10
        style: progressBarStyle

        PropertyAnimation {
            id: progressBarAnimation
            target: progressBar
            property: "value"
            from: mainWindow.realvalue
            to: mainWindow.realvalue ? 0 : 1
            duration: 200
        }
    }

    Component {
        id: progressBarStyle
        ProgressBarStyle {
            background: Rectangle {
                color: "black"
                border.color: "transparent"
            }
            progress: Rectangle {
                color: "white"
                border.color: "transparent"
                anchors.fill: parent
                anchors.margins: 1
                //radius: 50
            }
        }
    }

    Canvas {
        id: canvasMain

        width: parent.width
        height: parent.height
        anchors.left: parent.left
        anchors.top: parent.top
        contextType: "2d"

        onPaint: {

            /** 画笔转折样式

            lineJoin = 'miter' 尖角default 'bevel' 斜角 'round' 圆角
            lineCap = 'butt' 平default 'round' 圆，冒出一截，线的宽度 'square' 方，冒出一截，线的宽度

            */

            console.log("balloon Scale Height->" + mainWindow.height)
            //context.Antialiasing = true
            context.lineWidth = 3
            context.lineJoin = "bevel"
            //context.lineCap  = "square"
            context.strokeStyle = "white"
            context.fillStyle = "transparent"
            context.beginPath();
            context.moveTo(29, 5)
            context.lineTo(29, 13);
            context.lineTo(21, 33);
            context.lineTo(21, mainWindow.height - 33);
            context.lineTo(29, mainWindow.height - 13);
            context.lineTo(29, mainWindow.height - 5);
            context.lineTo(29, mainWindow.height - 13);
            context.lineTo(37, mainWindow.height - 33);
            context.lineTo(37, 33);
            context.lineTo(29, 13);
            context.closePath();
            context.fill()
            context.stroke();

            context.lineWidth = 2
            context.lineJoin  = "round"

            var leftMargin = 10
            var intervalValue = 19

            for (var i = 0; i < 9; i++) {
                context.beginPath();

                if (i == 0)
                {
                    context.moveTo(leftMargin, 12)
                    context.lineTo(20, 12);
                }
                else if (i == 8)
                {
                    context.moveTo(leftMargin, mainWindow.height - 12)
                    context.lineTo(20, mainWindow.height - 12);
                }
                else
                {
                    context.moveTo(leftMargin, 12 + intervalValue*i)
                    context.lineTo(13, 12 + intervalValue*i);
                }

                context.closePath();
                context.stroke();
            }

            context.lineWidth = 2
            context.strokeStyle = "black"
            context.beginPath();
            context.moveTo(26, 13)
            context.lineTo(18, 33);
            context.closePath();
            context.stroke();

            context.beginPath();
            context.moveTo(32, 13)
            context.lineTo(40, 33);
            context.closePath();
            context.stroke();

            context.beginPath();
            context.moveTo(32, mainWindow.height - 13)
            context.lineTo(40, mainWindow.height - 33);
            context.closePath();
            context.stroke();

            context.beginPath();
            context.moveTo(18, mainWindow.height - 33)
            context.lineTo(26, mainWindow.height - 13);
            context.closePath();
            context.stroke();
        }
    }

//    Timer {
//        id: xxx
//        interval: 2000   //设置定时器定时时间为500ms,默认1000ms
//        running: true  //是否开启定时，默认是false，当为true的时候，进入此界面就开始定时
//        repeat: true   //是否重复定时,默认为false
//        onTriggered: {
////            if (mainWindow.realvalue > 1.0)
////            {
////                //console.log("mainWindow.value->"+mainWindow.realvalue)
////                value = 0.0
////                mainWindow.realvalue = 0.0;
////            }
////            //console.log("mainWindow.value->"+mainWindow.realvalue)
////            mainWindow.realvalue = mainWindow.realvalue+0.1;
////            value = mainWindow.realvalue
//            mainWindow.realvalue = mainWindow.realvalue ? 0 : 1
//            progressBarAnimation.start()
//        }
//    }
}
