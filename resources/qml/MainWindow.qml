import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.settings 1.0
import QtQuick.Controls.Material 2.12

import Steamworks 1.0

import "./DirectoryView" as DirView

import "MainWindow.js" as JS

import "QSteamworks" as Steamworks

import DeckFM 1.0

ApplicationWindow {
    id: mainWindow

    property string openFile

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

        defaultActionSet: "deckfm"
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

    Shortcut {
        sequence: "F3"
        context: Qt.ApplicationShortcut

        onActivated: debugOverlay.toggle()
    }

    Steamworks.SteamInputScope {
        pressHandlers: {
            "debug": debugOverlay.toggle,
        }
    }

    Drawer {
        id: drawer
        y: header.height
        width: Math.max(parent.width * 0.3, 450)
        height: parent.height - header.height - footer.height
    }

    footer: Footer {
        visible: stackView.currentItem.showFooter
        hintActions: stackView.currentItem.hintActions
    }

    StackView {
        id: stackView
        focus: true

        anchors.fill: parent

        initialItem: directoryView

        DirView.DirectoryView {
            id: directoryView

            onFileOpened: {
                JS.openFile(path)
            }
        }
    }

    GlobalMenu {
        id: globalMenu

        onExitClicked: mainWindow.close()
    }

    DebugOverlay {
        id: debugOverlay

        x: 0
        y: 0
        width: parent.width - x
        height: parent.height - y
    }

    Settings {
        property alias path: directoryView.path
    }

    onOpenFileChanged: {
        JS.openFile(openFile)
    }
}
