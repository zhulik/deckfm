import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "QSteamworks" as Steamworks

import "debug" as Debug

Item {
    id: root

    signal closed()

    property var deckfmSettings: ({
                                      showCloseButton: false,
                                      showFooter: false
                                  })

    function toggle() {
        root.visible = !root.visible
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.6
    }

    Steamworks.SteamInputScope {
        id: input
        onInputEvent: {
            lastEventLabel.text = JSON.stringify(event, null, 2)
        }
    }

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: bar
            Layout.fillWidth: parent

            currentIndex: 1

            TabButton {
                text: "Controller state"
            }

            TabButton {
                text: "Console"
            }

            TabButton {
                text: "IGA"
            }
        }

        StackLayout {
            Layout.fillWidth: parent

            currentIndex: bar.currentIndex

            ColumnLayout {

                Label {
                    text: `Action set: ${input.actionSet}`
                }

                Label {
                    text: `Current layer: ${JSON.stringify(input.actionSetLayer)} (available: ${JSON.stringify(steam_input.currentActionSet.layers)})`
                }

                Label {
                    id: lastEventLabel
                }
            }

            Debug.Console{

            }

            Debug.IGAView {

            }
        }
    }
}
