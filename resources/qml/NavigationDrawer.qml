import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./MDI" as MDI
import "QSteamworks" as Steamworks

Drawer {
    id: drawer
    y: header.height
    width: Math.max(parent.width * 0.3, 450)
    height: stackView.height

    Shortcut {
        sequence: "F2"

        context: Qt.ApplicationShortcut

        onActivated: visible = !visible
    }

    SteamInputControl {
        objectName: "NavigationDrawer"
        actionSetLayers: ["tab_navigation"]

        z: 100
        pressHandlers: {
            "tab_navigation_top_left": () => console.log("!!!"),
            "tab_navigation_top_right": () => console.log("!!!")
        }
    }

    ColumnLayout {
        Steamworks.ActionIcon {
            name: "tab_navigation_top_left"
            width: 30
            height: 30
        }
        Steamworks.ActionIcon {
            name: "tab_navigation_top_right"
            width: 30
            height: 30
        }
        anchors.fill: parent

        TabView {
            model: [{
                    "name": "Files",
                    "icon": "file"
                }, {
                    "name": "Games",
                    "icon": "gamepad"
                }, {
                    "name": "Browser",
                    "icon": "web"
                }]
            tabDelegate: TabButton {
                MDI.Icon {
                    name: modelData.icon
                    anchors.centerIn: parent
                }
            }
        }
    }
}
