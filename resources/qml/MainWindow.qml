import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.settings 1.0

import QtQuick.Controls.Material 2.12

import Steamworks 1.0

import "./DirectoryView" as DirView

import "MainWindow.js" as JS

ApplicationWindow {
    id: mainWindow

    Material.theme: Material.Dark

    title: "DeckFM"
    visible: true

    visibility: {
        steamUtils.isSteamRunningOnSteamDeck
                || steamUtils.isSteamInBigPictureMode ? "FullScreen" : "Windowed"
    }

    width: 1280
    height: 800

    SteamUtils {
        id: steamUtils
    }

    SteamInput {
        id: steam_input // FIXME

        igaPath: {
            const url = Qt.resolvedUrl("../../input.vdf")

            url.slice(6, url.length)
        }

        defaultActionSet: "folder_navigation"
    }

    header: Header {
        id: header
        onMenuClicked: drawer.visible = !drawer.visible
        onLogoClicked: globalMenu.popup()
        onExitClicked: mainWindow.close()

        visible: stackView.depth == 1
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
                JS.openFile(path, mime)
            }
        }
    }

    GlobalMenu {
        id: globalMenu

        onExitClicked: mainWindow.close()
    }

    Settings {
        property alias path: directoryView.path
    }
}
