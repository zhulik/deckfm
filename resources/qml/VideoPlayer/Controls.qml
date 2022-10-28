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
        height: 45
        anchors.bottom: parent.bottom

        Rectangle {
            anchors.fill: parent
            color: "green"
            opacity: 0.4
        }

        Steamworks.SteamInputScope {
            id: input
            enabled: deckControlsEnabled

            pressHandlers: {
                "media_playpause": () => { video.playPause() },
            }
        }

        Keys.onPressed: {
            event.accepted = true

            switch (event.key) {
            case Qt.Key_Space:
                video.playPause()
                break
            }
        }


        MC.PositionSlider {
            id: slider
            width: parent.width

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
    }

    Label {
        visible: slider.pressed
        text: slider.timeToHuman(slider.selectedPosition - slider.moveStartPosition, true)
        font.pointSize: 128
        anchors.centerIn: parent
    }
}
