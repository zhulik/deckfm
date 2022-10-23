import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../QSteamworks" as Steamworks
import "MediaControls" as MC

Item {
    id: root

    property Video video
    property alias deckControlsEnabled: slider.deckControlsEnabled

    Item {
        width: parent.width
        height: 100
        anchors.bottom: parent.bottom


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

            Steamworks.SteamInputScope {
                id: input
                enabled: deckControlsEnabled
            }

            MC.PositionSlider {
                id: slider
                Layout.fillWidth: parent

                deckControlsEnabled: deckControlsEnabled

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

    Label {
        visible: slider.pressed
        text: slider.timeToHuman(slider.selectedPosition - slider.moveStartPosition, true)
        font.pointSize: 128
        anchors.centerIn: parent
    }
}
