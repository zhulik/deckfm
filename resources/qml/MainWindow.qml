import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.settings 1.0

import QtQuick.Controls.Material 2.12

import "./DirectoryView" as DirView

ApplicationWindow {
    id: mainWindow

    Material.theme: Material.Dark

    title: "DeckFM"
    visible: true

    visibility: {
        steam_utils.isSteamRunningOnSteamDeck
                || steam_utils.isSteamInBigPictureMode ? "FullScreen" : "Windowed"
    }

    width: 1280
    height: 800

    header: Header {
        id: header
        onMenuClicked: drawer.visible = !drawer.visible
        onLogoClicked: globalMenu.popup()
        onExitClicked: mainWindow.close()
        onGamepadClicked: gamepadWindow.open()

        visible: !gamepadWindow.visible && !appLoader.visible
    }

    Shortcut {
        sequence: "F1"
        context: Qt.ApplicationShortcut

        onActivated: globalMenu.visible = !globalMenu.visible
    }

    Shortcut {
        sequence: "F2"
        context: Qt.ApplicationShortcut

        onActivated: drawer.visible = !drawer.visible
    }

    Drawer {
        id: drawer
        y: header.height
        width: Math.max(parent.width * 0.3, 450)
        height: parent.height - header.height - footer.height
    }

    footer: Footer {
//        visible: !appLoader.visible
    }

    StackView {
        id: stackView
        focus: true

        anchors.fill: parent

        initialItem: directoryView

        DirView.DirectoryView {
            id: directoryView

            onFileOpened: {
                console.log(mime)
                appLoader.source = path
            }
        }

        AppLoader {
            id: appLoader

            onClosed: {
                stackView.pop()
            }

            onLoaded: {
                stackView.push(appLoader)
            }
        }
    }

    GlobalMenu {
        id: globalMenu

        onExitClicked: mainWindow.close()
        onGamepadClicked: gamepadWindow.open()
    }

    GamepadWindow {
        id: gamepadWindow

        x: 0
        y: 0
        width: parent.width - x
        height: parent.height - y
    }

    Settings {
        property alias path: directoryView.path
    }
}
