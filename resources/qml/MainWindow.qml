import QtQuick 2.15
import QtQuick.Window 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import Qt.labs.settings 1.0
import QtQuick.Controls.Material 2.12

import Steamworks 1.0
import Steamworks.SteamInput 1.0

import "./DirectoryView" as DirView
import "./GamesView" as GamesView

import "MainWindow.js" as JS

import "QSteamworks" as Steamworks

import DeckFM 1.0

ApplicationWindow {
    id: mainWindow

    property string openFile

    Material.theme: Material.Dark

    title: "DeckFM"

    visibility: {
        steamUtils.isSteamRunningOnSteamDeck
                || steamUtils.isSteamInBigPictureMode ? "FullScreen" : "Windowed"
    }

    width: 1280
    height: 720

    header: Header {
        id: header
        onMenuClicked: navigationDrawer.visible = !navigationDrawer.visible
        onLogoClicked: globalMenu.popup()
        onExitClicked: mainWindow.close()
    }

    footer: Footer {
        visible: swipeView.currentItem.showFooter
        hintActions: swipeView.currentItem.hintActions
    }

    SteamUtils {
        id: steamUtils
    }

    SteamInput {
        id: steam_input // FIXME

        igaPath: {
            const url = Qt.resolvedUrl("../../input.vdf")

            url.slice(6, url.length)
        }
    }

    Shortcut {
        sequence: "F1"
        context: Qt.ApplicationShortcut

        onActivated: globalMenu.visible = !globalMenu.visible
    }

    Shortcut {
        sequence: "F4"
        context: Qt.ApplicationShortcut

        onActivated: Qt.quit()
    }

    SteamInputScope {
        id: steam_input_scope

        anchors.fill: parent

        SteamInputControl {
            global: true

            objectName: "global"

            controller: steam_input.lastController

            pressHandlers: {
                "debug": () => debugOverlay.visible = !debugOverlay.visible,
                "select": () => navigationDrawer.visible = !navigationDrawer.visible
            }
        }

        NavigationDrawer {
            id: navigationDrawer
            y: header.height
            width: Math.max(parent.width * 0.3, 450)
            height: swipeView.height

            onCurrentModeChanged: {
                const cs = swipeView.contentChildren
                for (var i = 0; i < cs.length; i++) {
                    if (cs[i].tabName === currentMode) {
                        swipeView.currentIndex = i
                    }
                }
            }
        }

        SwipeView {
            id: swipeView
            focus: true
            anchors.fill: parent

            onCurrentIndexChanged: {
                navigationDrawer.setCurrentMode(
                            contentChildren[currentIndex].tabName)
            }

            DirView.DirectoryView {
                id: directoryView

                readonly property string tabName: "file_manager"

                onFileOpened: JS.openFile(path)
                visible: swipeView.currentItem == directoryView
            }

            GamesView.GamesView {
                id: gamesView

                readonly property string tabName: "games"
            }
        }

        GlobalMenu {
            id: globalMenu

            onExitClicked: mainWindow.close()
        }

        DebugOverlay {
            id: debugOverlay
            visible: false

            x: 0
            y: 0
            width: parent.width - x
            height: parent.height - y
        }

        Settings {
            property alias path: directoryView.path
        }
    }

    onOpenFileChanged: JS.openFile(openFile)
}
