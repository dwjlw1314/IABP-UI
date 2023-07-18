import QtQuick 2.0

Rectangle {

    opacity: 1
    property int behduration: 100

    property string first:  "#F3F7FF"
    property string second: "#D1D3F8"

    radius: 12

    gradient: Gradient {
        GradientStop { position: 0.0; color: first }
        //GradientStop { position: 0.5; color: "lightslategraY" }
        GradientStop { position: 1.0; color: second }
    }

    /** //动画效果
    Behavior on opacity {
        NumberAnimation {
            duration: behduration
        }
    }

    Behavior on height {
        NumberAnimation {
            duration: behduration
        }
    }
    */
}
