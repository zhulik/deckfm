import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "./DirectoryView" as DirView

ApplicationWindow {
    id: mainWindow

    Material.theme: Material.Dark

    title: "DeckFM"
    visible: true

    visibility: "Windowed"

    width: 1280
    height: 800

    Gamepad {
        id: gamepad
    }

    header: Header {
        onMenuClicked: {
            drawer.visible = !drawer.visible
        }

        onLogoClicked: {
            globalMenu.popup()
        }

        onExitClicked: {
            mainWindow.close()
        }
    }

    Shortcut {
        sequence: "F1"
        context: Qt.ApplicationShortcut

        onActivated: {
            globalMenu.visible = !globalMenu.visible
        }
    }

    Shortcut {
        sequence: "F2"
        context: Qt.ApplicationShortcut

        onActivated: {
            drawer.visible = !drawer.visible
        }
    }

    Drawer {
        id: drawer
        y: header.height
        width: Math.max(parent.width * 0.3, 450)
        height: parent.height - header.height - footer.height

        onClosed: {
            directoryViewLeft.forceActiveFocus()
        }
    }

    footer: Footer {}

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            Layout.margins: {
                left: 10
                right: 10
            }

            Layout.fillWidth: true
            Layout.fillHeight: true

            DirView.DirectoryView {
                id: directoryViewLeft
                Layout.fillWidth: true
                Layout.fillHeight: true
                focus: true

                onFileOpened: {
                    const mime = fs_bridge.mime(url)
                    console.log(mime[0])
                }
            }
        }
    }

    GlobalMenu {
        id: globalMenu

        onExitClicked: mainWindow.close()

        onClosed: {
            directoryViewLeft.forceActiveFocus()
        }
    }

    Component.onCompleted: {
        console.log(steam_api_bridge.SteamUtils())
    }
}
