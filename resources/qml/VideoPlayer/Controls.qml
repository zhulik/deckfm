import QtQuick 2.15
import QtMultimedia 5.15 as M
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../QSteamworks" as Steamworks
import "MediaControls" as MC

Item {
    id: root

    property Video video
    property alias deckControlsEnabled: slider.deckControlsEnabled

    Item {
        anchors.fill: parent

        Rectangle {
            anchors.fill: parent
            color: "black"
            opacity: 0.4
        }

        Steamworks.SteamInputScope {
            id: input
            enabled: deckControlsEnabled

            pressHandlers: {
                "media_playpause": video.playPause
            }
        }

        MC.Buttons {
            height: 80
            spacing: 60
            playing: video.playbackState === M.MediaPlayer.PlayingState

            anchors.centerIn: parent
            onPlayPausePressed: {
                video.playPause()
            }
        }

        Keys.onPressed: {
            switch (event.key) {
            case Qt.Key_Space:
                event.accepted = true
                video.playPause()
                break
            }
        }


        MC.PositionSlider {
            id: slider

            width: parent.width
            anchors.bottom: parent.bottom

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
