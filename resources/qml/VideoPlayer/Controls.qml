import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "MediaControls" as MC

Item {
    id: root

    property Video video

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

        MC.PositionSlider {
            id: slider
            Layout.fillWidth: parent

            position: video.position
            duration: video.duration

            onPressedChanged: {
                if (pressed) {
                    video.pause()
                } else {
                    video.play()
                }
            }

            onSeek: {
                video.seek(position, debounce)
            }
        }

        Item {
            Layout.fillHeight: parent
        }
    }
}
