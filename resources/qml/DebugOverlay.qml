import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "QSteamworks" as Steamworks

import Steamworks.SteamInput 1.0

import "debug" as Debug

Item {
    id: root

    signal closed

    property var deckfmSettings: ({
                                      "showCloseButton": false,
                                      "showFooter": false
                                  })

    function toggle() {
        root.visible = !root.visible
    }

    SteamInputControl {
        objectName: "DebugOverlayInput"
        controller: steam_input.lastController

        enabled: visible

        actionSet: "deckfm"
        actionSetLayers: []
    }

    Rectangle {
        anchors.fill: parent
        color: "black"
        opacity: 0.6
    }

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: bar
            Layout.fillWidth: parent

            currentIndex: 2

            TabButton {
                text: "Console"
            }

            TabButton {
                text: "IGA"
            }

            TabButton {
                text: "Controllers"
            }
        }

        StackLayout {
            Layout.fillWidth: parent

            currentIndex: bar.currentIndex

            Debug.ConsoleTab {}

            Debug.IGATab {}

            Debug.ControllersTab {}
        }
    }
}
