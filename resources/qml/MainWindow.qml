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

    visibility: { steam_utils.isSteamRunningOnSteamDeck || steam_utils.isSteamInBigPictureMode ? "FullScreen" : "Windowed" }

    width: 1280
    height: 800

    header: Header {
        onMenuClicked: drawer.visible = !drawer.visible
        onLogoClicked: globalMenu.popup()
        onExitClicked: mainWindow.close()
        onGamepadClicked: gamepadWindow.open()
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
                id: directoryView
                Layout.fillWidth: true
                Layout.fillHeight: true
                focus: true

                onFileOpened: {
                    console.log(`Attempting to open ${path}`)
                }
            }
        }
    }

    GlobalMenu {
        id: globalMenu

        onExitClicked: mainWindow.close()
        onGamepadClicked: gamepadWindow.open()
    }

    Component.onCompleted: {
//        gamepadWindow.open()
        const iga = steam_input.iga
        console.log(iga.actionsForSet(iga.actionSets[0]));
    }

    GamepadWindow {
        id: gamepadWindow

        x: 0
        y: 0
        width: parent.width - x
        height: parent.height - y
    }
}
