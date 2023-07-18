import QtQuick 2.0

Item {
    id: mainWindow

    readonly property int verLineheight: 10

    property string fontColor: "white"
    property string textValue: "Inflation"
    property string showValue: "0"

    property int intervalValue: 30
    property int intervalNums: 8

    property int slidingAsexX: 0
    property int slidingAreaW: 40

    Rectangle{
        id: mainArea
        anchors.left: mainWindow.left
        anchors.top: mainWindow.top
        width: mainWindow.width
        height: mainWindow.height
        color: "transparent"

        Text {
            text: textValue
            color: fontColor
            font.pointSize: 13
            font.bold: true
            anchors.left: mainArea.left
            anchors.leftMargin: 10
            anchors.top: mainArea.top
        }

        Canvas{ //圆角矩形
            id:canvasLine
            x: 10
            y: 8
            width: 260
            height: 50
            antialiasing: true

            onPaint: {
                var context = canvasLine.getContext("2d");
                var x = 2;
                var y = 20;

                var radius = verLineheight;

                context.lineWidth = 2;
                context.lineCap = "round";
                context.strokeStyle = fontColor;

                context.beginPath();
                context.moveTo(x, y+verLineheight/2+1);
                context.lineTo(x+intervalNums*intervalValue, y+verLineheight/2+1);
                context.moveTo(x, y);
                for (var i = 0; i < intervalNums; i++) {
                    context.lineTo(x, y+radius);
                    x += intervalValue;
                    if (i === intervalNums/2-1 || i === intervalNums-1)
                    {
                        radius = verLineheight;
                        context.moveTo(x, y);
                    }
                    else
                    {
                        radius = 6;
                        context.moveTo(x, y+(radius++)/2);
                    }
                }
                context.lineTo(x, y+radius);

                context.stroke();
            }
        }

        Rectangle {
            id: slidingArea
            x: mainWindow.slidingAsexX > intervalNums*intervalValue - mainWindow.slidingAreaW
                ? intervalNums*intervalValue - mainWindow.slidingAreaW + 9 : slidingAsexX + 12
            y: canvasLine.y + 17
            width: mainWindow.slidingAreaW  //是否需要变
            height: 16
            color: fontColor
            radius: 4
            border.width: 0

            Text {
                text: mainWindow.showValue
                color: "#808080"
                font.pointSize: 8
                font.bold: true
                anchors.centerIn: parent
            }
        }
    }
}
