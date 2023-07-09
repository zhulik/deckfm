import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../../MDI" as MDI
import "../../" as Core

Core.Spoiler {
    id: root
    text: "Vibration"
    collapsed: true

    property var controller

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
                stepSize: 1
            }
        }

        MDI.Button {
            iconName: "vibrate"

            onClicked: {
                root.controller.triggerRepeatedHapticPulse(
                            usDurationMicroSecSlider.value,
                            usOffMicroSecSlider.value, unRepeatSlider.value)
            }
        }
    }
}
