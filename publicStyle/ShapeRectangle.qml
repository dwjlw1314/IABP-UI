import QtQuick 2.0
//import QtQuick.Controls 2.12
//import QtQuick.Shapes 1.12

Canvas{ //圆角矩形
    id: canvasLine
    x: 0
    y: 0
    height: 25
    antialiasing: true

    property int topWidth: parent.width
    property int leftHeigth: 17
    property string leftText: ""
    property string midText: ""
    property string rightText: ""
    property string textColor: ""
    property int rightTextSize: 10
    property bool drawLine: false

    onPaint: {
        var context = canvasLine.getContext("2d");
        var x = 5; // 圆角矩形左上角横坐标
        var y = 5; // 圆角矩形左上角纵坐标
        var width = 16; // 圆角矩形的宽度
        var height = 16; // 圆角矩形的高度
        var radius = 8; // 圆角的半径
        context.lineWidth = 2;

        //context.lineCap = "round";
        // 开始创建新路径
        context.beginPath();
        if (drawLine)
        {
            context.moveTo(x, y);
            context.lineTo(topWidth + x, y);
        }
        else
        {
            context.lineTo(x, y+leftHeigth);
            // 绘制左上角圆角
            context.arc(x + radius, y + radius, radius, Math.PI, Math.PI * 3 / 2);
            // 绘制顶边路径
            context.lineTo(topWidth + x, y);
        }
        /*
        // 绘制右上角圆角
        context.arc(width - radius + x, radius + y, radius, Math.PI * 3 / 2, Math.PI * 2);
        // 绘制右边路径
        context.lineTo(width + x, height + y - radius);
        // 绘制右下角圆角
        context.arc(width - radius + x, height - radius + y, radius, 0, Math.PI * 1 / 2);
        // 绘制底边路径
        context.lineTo(radius + x, height +y);
        // 绘制左下角圆角
        context.arc(radius + x, height - radius + y, radius, Math.PI * 1 / 2, Math.PI);
        // 闭合路径 也可使用 context.lineTo(x, y + radius);
        context.closePath();
        */
        // 设置绘制的颜色
        context.strokeStyle = '#CBCCE4';
        context.stroke();
    }

    Text {
        text: leftText
        font.pointSize: 12
        color: textColor
        font.bold: true
        //font.family: "Helvetica"

        antialiasing: true
        anchors.left: parent.left
        anchors.leftMargin: 10
        anchors.top: parent.top
        anchors.topMargin: 5
    }

    Text {
        text: midText
        font.pointSize: 12
        color: textColor
        font.bold: true
        //font.family: "Helvetica"

        antialiasing: true
        anchors.left: parent.left
        anchors.leftMargin: 180
        anchors.top: parent.top
        anchors.topMargin: 5
    }

    Text {
        text: rightText
        font.pointSize: rightTextSize
        color: textColor
        font.bold: true
        //font.family: "Helvetica"

        antialiasing: true
        anchors.right: parent.right
        anchors.rightMargin: 8
        anchors.top: parent.top
        anchors.topMargin: 5
    }
}

//Shape {
//    id: shape
//    property var cornersRadius
//    property color color
//    property color borderColor:"transparent"
//    property int borderWidth: 1
//    layer.enabled: true
//    layer.samples: 4
//    layer.smooth: true
//    antialiasing: true

//    ShapePath {
//        startX: 0
//        startY: cornersRadius[0]
//        fillColor: color
//        strokeColor: borderColor
//        strokeWidth: borderWidth
//        PathQuad { x: cornersRadius[0]; y: 0; controlX: 0; controlY: 0 }
//        PathLine { x: shape.width - cornersRadius[1]; y: 0 }
//        PathQuad { x: shape.width; y: cornersRadius[1]; controlX: shape.width; controlY: 0 }
//        PathLine { x: shape.width; y: shape.height - cornersRadius[2] }
//        PathQuad { x: shape.width - cornersRadius[2]; y: shape.height; controlX: shape.width; controlY: shape.height }
//        PathLine { x: cornersRadius[3]; y: shape.height }
//        PathQuad { x: 0; y: shape.height - cornersRadius[3]; controlX: 0; controlY: shape.height }
//        PathLine { x: 0; y: cornersRadius[0] }
//    }
//}
