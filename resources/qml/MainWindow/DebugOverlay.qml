import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import Steamworks.SteamInput 1.0

import "./DebugOverlay" as Components

Item {
    id: root

    signal closed

    property var deckfmSettings: ({
                                      "showCloseButton": false,
                                      "showFooter": false
                                  })

    Shortcut {
        sequence: "F3"
        context: Qt.ApplicationShortcut

        onActivated: visible = !visible
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

            Components.ConsoleTab {}

            Components.IGATab {}

            Components.ControllersTab {}
        }
    }
}
