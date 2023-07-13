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

    signal fileOpened(string path)

    property alias path: fs_model.path

    property bool showFooter: true
    property var hintActions: ["scroll", "file_manager_open", "file_manager_go_back", "file_manager_go_home"]

    FolderListModel {
        id: fs_model
        showHidden: showHiddenSwitch.position === 1.0
        onModelReset: view.currentIndex = 0

        function cdIndex(index) {
            const item = fs_model.get(index)

            if (item.isDir) {
                fs_model.path = item.path
                return
            }
            root.fileOpened(item.path)
        }
    }

    onActiveFocusChanged: view.forceActiveFocus()

    ColumnLayout {
        anchors.fill: parent

        RowLayout {
            FilePathView {
                pathComponents: fs_model.pathComponents

                Layout.preferredHeight: 40
                Layout.preferredWidth: 500

                onPathSelected: {
                    fs_model.path = path
                }
            }

            Item {
                Layout.fillWidth: parent
            }

            Switch {
                id: showHiddenSwitch
                text: "Show hidden"
                focusPolicy: Qt.NoFocus
            }

            MDI.Button {
                iconName: "arrowUp"
                onClicked: {
                    fs_model.goUp()
                }
                enabled: fs_model.canGoUp
            }

            MDI.Button {
                iconName: "home"
                onClicked: {
                    fs_model.goHome()
                }
            }
        }

        Core.SteamInputScrollableGridView {
            id: view

            model: fs_model

            Layout.fillWidth: true
            Layout.fillHeight: true

            cellWidth: Math.max(parent.width / 4, 300)
            cellHeight: 70

            actionSetLayers: ["file_manager"]

            pressHandlers: {
                "file_manager_open": () => fs_model.cdIndex(view.currentIndex),
                "file_manager_go_back": fs_model.goUp,
                "file_manager_go_home": fs_model.goHome
            }

            delegate: FileDelegate {
                width: view.cellWidth
                height: view.cellHeight

                onClicked: {
                    fs_model.cdIndex(index)
                }
            }

            Keys.onPressed: {
                switch (event.key) {
                case Qt.Key_Return:
                    event.accepted = true
                    fs_model.cdIndex(view.currentIndex)
                    break
                case Qt.Key_Escape:
                    event.accepted = true
                    fs_model.goUp()
                    break
                case Qt.Key_Home:
                    event.accepted = true
                    fs_model.goHome()
                    break
                }
            }
        }

        RowLayout {
            Label {
                text: `Total: ${fs_model.count}`
                font.pointSize: 14
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
