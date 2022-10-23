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
                const abs = Math.abs(e.analogX)

                if (abs <= 1) {
                    return
                }

                const s = e.analogX / abs

                const acceleration = abs / 100 + 1

                slider.value = slider.value + Math.pow(abs, acceleration) * s * 100
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
            to: duration
            stepSize: 100

            function timeToHuman(mils) {
                let secs = Math.round(mils / 1000);
                let mins = Math.floor(secs / 60)
                secs = String(secs % 60).padStart(2, '0')
                const hours = Math.floor(mins / 60)
                mins = String(mins % 60).padStart(2, '0')

                if (hours == 0) {
                    return `${mins}:${secs}`
                }

                return `${String(hours).padStart(2, '0')}:${mins}:${secs}`
            }

            onValueChanged: root.seek(value, true)
        }
    }

    Label {
        text: slider.timeToHuman(position)

        y: 0
        x: slider.visualPosition * slider.width - width / 2
    }

    onPositionChanged: {
        if (!root.pressed) {
            slider.value = position
            return
        }
    }

    onPressedChanged: {
        if (!pressed) {
            root.seek(slider.value, false)
        }
    }

}
