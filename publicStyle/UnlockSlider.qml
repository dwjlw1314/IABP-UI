import QtQuick 2.0
import QtGraphicalEffects 1.0
/**   网络组件   **/

Item {
    id: shell
    readonly property real visualPosition: brick.x / (width - brick.width)
    property string hint: "滑动解锁"
    property alias sourceLock: lockIcon.source
    property alias sourceUnlock: unlockIcon.source
    signal unlock

    implicitWidth: 400; implicitHeight: 70

    Rectangle{
        id: bg
        anchors.fill: parent
        radius: height * 0.5
        color: "white"
        opacity: 0.5
    }

    /****************   提示文字    ***************/
    Text{
        id: backText
        anchors.centerIn: parent
        opacity: 0.8
        text: hint
        font.pixelSize: shell.height * 0.5
        color: "gray"
    }
    Text{
        id: foreText
        visible: false
        anchors.centerIn: parent
        text: hint
        font.pixelSize: backText.font.pixelSize
        color: "white"
    }
    OpacityMask{
        anchors.centerIn: parent
        width: foreText.contentWidth; height: foreText.contentHeight
        source: foreText
        maskSource: maskArea
    }
    RadialGradient{     //创建一个透明度渐变的椭圆来做 OpacityMask 的mask
        id: maskArea
        visible: false
        width: foreText.contentWidth; height: foreText.contentHeight
        anchors.left: parent.left
        gradient: Gradient {
            GradientStop{position: 0.1; color: "red"}
            GradientStop{position: 0.8; color: "transparent"}
        }
    }
    PropertyAnimation{  //改变mask的水平偏移来使mask移动
       target: maskArea
       properties: 'horizontalOffset'
       from: -maskArea.width
       to: maskArea.width + maskArea.horizontalRadius
       duration: 2000
       running: true
       loops: -1
    }

    /***************   滑块    ***************/
    Item{
        id: brick
        width: parent.height * 0.8; height: width
        anchors.verticalCenter: parent.verticalCenter
        x: shell.height - height
        Rectangle{
            id: brickBg1
            anchors.fill: parent
            radius: height * 0.5
            color: "white"
            opacity: 1 - shell.visualPosition
        }
        Rectangle{
            id: brickBg2
            anchors.fill: parent
            radius: height * 0.5
            color: "lightgreen"
            opacity: shell.visualPosition
        }
        Image {
            id: lockIcon
            anchors.centerIn: parent
            source: ""
            sourceSize.height: parent.height * 0.7
            opacity: 1 - shell.visualPosition
        }
        Image {
            id: unlockIcon
            visible: visualPosition > 0.5
            anchors.centerIn: parent
            source: ""
            sourceSize.height: parent.height * 0.7
            opacity: shell.visualPosition > 0.5 ? 2*(shell.visualPosition-0.5): 0
        }
        MouseArea{
            id: mouseArea
            drag.target: brick
            drag.minimumX: 0; drag.maximumX: shell.width - width
            drag.minimumY: 0; drag.maximumY: 0
            anchors.fill: brick
            onReleased: {
                if(brick.x+brick.width < shell.width * 0.9){    //没触发
                    brick.x = shell.height - height
                }
                else {          //触发
                    brick.x = shell.width - width - (shell.height - height)
                    delayEmitTimer.start()
                }
            }
        }
        Behavior on x {
            PropertyAnimation {
                id: xAni
                duration: 200
            }
        }
        Timer{
            id: delayEmitTimer
            interval: xAni.duration + 150
            onTriggered: {
                delayEmitTimer.stop()
                shell.unlock()
                brick.x = shell.height - mouseArea.height   //触发后滑块复位
            }
        }
    }
    /***************   滑块终点    ***************/
    Rectangle{
        id: fantasy
        width: parent.height * 0.8; height: width
        radius: height * 0.5
        anchors.verticalCenter: parent.verticalCenter
        x: shell.width - width - (shell.height - height)
        color: "transparent"
        border{color: "lightgreen"; width: 1}
        opacity: shell.visualPosition > 0.5 ? 2*(shell.visualPosition-0.5): 0
        Image {
            visible: visualPosition > 0.5
            anchors.centerIn: parent
            source: unlockIcon.source
            sourceSize.height: parent.height * 0.7
            opacity: shell.visualPosition > 0.5 ? 2*(shell.visualPosition-0.5): 0
        }
    }
}
