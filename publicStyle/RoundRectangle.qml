import QtQuick 2.0

/** @bref set bgColor, radius, radiusCorners */

Rectangle {
    id: root
    height:100
    width:200
    antialiasing: true
    property string title : ""
    property int textSize : 20
    property color bgColor : "black"
    property color textColor : "black"
    color: bgColor
    property int radiusCorners: Qt.AlignLeft | Qt.AlignRight | Qt.AlignTop | Qt.AlignBottom /* Default: */

    Repeater {
        model: [ {
                x: 0,
                y: 0,
                visible: internal.aligns(Qt.AlignLeft | Qt.AlignTop),
                radius: root.radius
            },
            {
                x: root.width - root.radius,
                y: 0,
                visible: internal.aligns(Qt.AlignRight | Qt.AlignTop),
                radius: root.radius
            },
            {
                x: 0,
                y: root.height - root.radius,
                visible: internal.aligns(Qt.AlignLeft | Qt.AlignBottom),
                radius: root.radius
            },
            {
                x: root.width - root.radius,
                y: root.height - root.radius,
                visible: internal.aligns(Qt.AlignRight | Qt.AlignBottom),
                radius: root.radius
            } ]

        Rectangle {
            x: modelData.x; y: modelData.y
            width: modelData.radius; height: width
            visible: !modelData.visible
            color: parent.color
        }
    }
    QtObject {
        id: internal

        function aligns(direction) {
            return (root.radiusCorners & direction) === direction
        }
    }

    Text {
        id:content
        text: title
        font.pixelSize: textSize
        color: textColor
        anchors.centerIn: parent
    }
}
