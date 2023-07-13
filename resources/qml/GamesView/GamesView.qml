import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

import QtQuick.Controls.Material 2.12

import DeckFM 1.0
import Steamworks.SteamInput 1.0

import "../MDI" as MDI
import ".." as Core

import "../QSteamworks" as Steamworks

Item {
    id: root

    property bool showFooter: true
    property var hintActions: ["scroll", "file_manager_open", "file_manager_go_back", "file_manager_go_home"]

    GridView {
        id: view
        anchors.fill: parent

        model: SteamGamesModel {
            id: fames_model
            onModelReset: view.currentIndex = 0
        }

        cellWidth: 150
        cellHeight: 210
        clip: true
        keyNavigationWraps: false

        ScrollBar.vertical: ScrollBar {}

        delegate: GameDelegate {
            width: view.cellWidth
            height: view.cellHeight
        }

        highlight: Rectangle {
            color: Material.primary
        }
    }
}
