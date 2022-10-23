import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../../QSteamworks" as Steamworks

Rectangle {
    id: root

    property int duration
    property int position

    property bool deckControlsEnabled: false

    property bool pressed: slider.pressed || input.actionStates["media_seek_control"] || false

    signal seek(int position, bool debounce)

    Steamworks.SteamInputScope {
        id: input
        enabled: deckControlsEnabled

        analogHandlers: {
            "media_seek": (e) => {
                const dx = e.analogX

                if (input.actionStates["media_seek_control"]) {
                    if (e.analogX > 5) {
                        slider.value = slider.value + x
                    }
                } else {
                    slider.value = slider.value + x
                }
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: parent

            Label {
                text: slider.timeToHuman(0)
            }

            Item {
                Layout.fillWidth: parent
            }

            Label {
                text: slider.timeToHuman(duration)
            }
        }

        Slider {
            id: slider
            Layout.fillWidth: parent

            from: 0
            to: duration / 1000

            function timeToHuman(mils) {
                let secs = Math.round(mils / 1000);
                let mins = Math.floor(secs / 60)
                secs = secs % 60
                const hours = Math.floor(mins / 60)
                mins = mins % 60

                return `${hours}:${mins}:${secs}`

            }

            onPressedChanged: {
                if (!pressed) {
                    root.seek(value * 1000, false)
                }
            }

            onValueChanged: root.seek(value * 1000, true)
        }
    }

    Label {
        text: slider.timeToHuman(position)

        y: 0
        x: slider.visualPosition * slider.width - width / 2
    }

    onPositionChanged: {
        if (!slider.pressed) {
            slider.value = position / 1000
            return
        }
    }

}