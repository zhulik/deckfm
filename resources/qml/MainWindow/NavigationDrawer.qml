import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../Components" as Core

Drawer {
    id: root

    property string currentMode: tabRepeater.model[tabBar.currentIndex].mode

    function setCurrentMode(mode) {
        if (mode === currentMode) {
            return
        }

        tabBar.currentIndex = tabRepeater.model.findIndex(x => x.mode === mode)
    }

    Shortcut {
        sequence: "F2"

        context: Qt.ApplicationShortcut

        onActivated: visible = !visible
    }

    Core.SteamInputControl {
        objectName: "NavigationDrawer"
        actionSetLayers: ["tab_navigation"]

        z: 100
        pressHandlers: {
            "tab_navigation_top_left": tabBar.decrementCurrentIndex,
            "tab_navigation_top_right": tabBar.incrementCurrentIndex,
            "cancel": root.close
        }
    }

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.fillWidth: parent

            Core.ActionIcon {
                name: "tab_navigation_top_left"
                width: 30
                height: 30
            }

            TabBar {
                id: tabBar
                Layout.fillWidth: parent

                Repeater {
                    id: tabRepeater
                    anchors.fill: parent

                    model: [{
                            "name": "Files",
                            "icon": "file",
                            "mode": "file_manager"
                        }, {
                            "name": "Games",
                            "icon": "gamepad",
                            "mode": "games"
                        }, {
                            "name": "Browser",
                            "icon": "web",
                            "mode": "browser"
                        }]

                    TabButton {
                        Core.MDIIcon {
                            name: modelData.icon
                            anchors.centerIn: parent
                        }
                    }
                }
            }

            Core.ActionIcon {
                name: "tab_navigation_top_right"
                width: 30
                height: 30
            }
        }

        Item {
            Layout.fillHeight: parent
        }
    }
}
