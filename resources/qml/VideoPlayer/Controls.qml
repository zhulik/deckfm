import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: root

    property var video

    opacity: 0

    function show() {
        opacity = 1
        hideTimer.restart()
    }

    function hide() {
        opacity = 0
    }

    Timer {
        id: hideTimer
        interval: 2000

        running: opacity > 0


        onTriggered: hide()
    }

    Rectangle {
        anchors.fill: parent
        color: "green"
        opacity: 0.4
    }

    ColumnLayout {
        anchors.fill: parent

        Label {
            text: `Error: ${video.errorString}`
        }

    }
}
