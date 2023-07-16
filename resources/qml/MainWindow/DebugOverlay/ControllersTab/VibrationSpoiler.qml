import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../../Components" as Core

Core.Spoiler {
    text: "Vibration"
    collapsed: true

    RowLayout {

        ColumnLayout {
            Layout.fillWidth: parent

            LabeledSlider {
                id: usDurationMicroSecSlider
                Layout.fillWidth: parent

                from: 1
                to: 65535
                value: 2900
                stepSize: 10
            }

            LabeledSlider {
                id: usOffMicroSecSlider

                Layout.fillWidth: parent
                from: 1
                to: 65535
                stepSize: 10
                value: 1200
            }

            LabeledSlider {
                id: unRepeatSlider

                Layout.fillWidth: parent
                from: 1
                to: 100
                value: 4
            }
        }

        Core.MDIButton {
            iconName: "vibrate"

            onClicked: {
                steam_input.lastController.triggerRepeatedHapticPulse(
                            usDurationMicroSecSlider.value,
                            usOffMicroSecSlider.value, unRepeatSlider.value)
            }
        }
    }
}
