import QtQuick 2.0

//弹窗的“按钮”，此处是因为使用了图标所以用Rectangle来实现
Rectangle {
    id: popUpBtn
    color: "#00000000"
    width: 30
    height: 30

    Image {
        anchors.centerIn: parent
        source: "qrc:/windowPicture/upPopButton.png"

        MouseArea {
            anchors.fill: parent

            onPressed: {
                // 此处是为了体现”按钮“点击时的背景效果
                popUpBtn.color = "#ff40598a"
            }

            onReleased: {
                // 恢复按钮未被按下时的背景效果
                popUpBtn.color = "#00000000"
            }

            onClicked: {

            }
        }
    }
}

