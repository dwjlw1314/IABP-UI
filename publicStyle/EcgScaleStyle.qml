import QtQuick 2.0

Canvas{ //圆角矩形
    id:canvasLine

    antialiasing: true

    property string lineColor: "#CBCCE4"
    property int intervalValue: 30
    property int intervalNums: 1

    readonly property int horLineWidth: 5

    onPaint: {
        var context = canvasLine.getContext("2d");
        var x = 35;
        var y = 1;
        context.lineWidth = 2;

        context.lineCap = "round";
        context.strokeStyle = lineColor;
        context.beginPath();
        context.moveTo(x+1, y)
        for (var i = 0; i < intervalNums; i++) {
            context.lineTo(x + horLineWidth, y);
            context.moveTo(x, y)
            y += intervalValue;
            if (i+1 == intervalNums)
                context.lineTo(x, y-1);
            else
                context.lineTo(x, y);
        }

        context.lineTo(x + horLineWidth, y-1);

        context.stroke();
    }
}
