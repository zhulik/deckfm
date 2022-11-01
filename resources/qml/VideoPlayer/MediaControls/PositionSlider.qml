import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../../QSteamworks" as Steamworks

Item {
    id: root

    height: layout.height

    property alias duration: slider.to
    property int position

    property int moveStartPosition: 0
    readonly property alias selectedPosition: slider.value

    property alias deckControlsEnabled: input.enabled

    readonly property bool pressed: slider.pressed || input.actionStates["media_seek_control"] || false

    signal seek(int position, bool debounce)

    function timeToHuman(mils, includeSign = false) {
        const abs = Math.abs(mils)

        let sign = mils / abs >= 0 ? "+" : "-"
        sign = includeSign ? sign : ""

        mils = abs

        let secs = Math.round(mils / 1000);
        let mins = Math.floor(secs / 60)
        secs = String(secs % 60).padStart(2, '0')
        const hours = Math.floor(mins / 60)
        mins = String(mins % 60).padStart(2, '0')

        if (hours == 0) {
            return `${sign}${mins}:${secs}`
        }

        return `${sign}${String(hours).padStart(2, '0')}:${mins}:${secs}`
    }

    Steamworks.SteamInputScope {
        id: input
        enabled: deckControlsEnabled

        analogHandlers: {
            "scroll": (e) => {
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
        id: layout
        spacing: -10
        width: parent.width
//        anchors.fill: parent


        Label {
            font.pointSize: 14
            text: `${root.timeToHuman(position)} / ${root.timeToHuman(duration)}`
        }

        Slider {
            id: slider
            Layout.fillWidth: parent

            from: 0
            to: duration
            stepSize: 100

            onValueChanged: root.seek(value, true)
        }
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
            moveStartPosition = 0
        } else {
            moveStartPosition = position
        }
    }

}
