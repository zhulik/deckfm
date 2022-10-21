import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

import QtQuick.Controls.Material 2.12

import DeckFM 1.0

import "../MDI" as MDI
import ".." as Core

import "../QSteamworks" as Steamworks

Item {
    id: root

    signal fileOpened(string path)

    property alias path: fs_model.path

    FolderListModel {
        id: fs_model
        showHidden: showHiddenSwitch.position === 1.0
        onModelReset: view.currentIndex = 0
    }

    function cdIndex(index) {
        if (fs_model.get(index).isDir) {
            fs_model.path = fs_model.get(index).path
            return
        }
        root.fileOpened(fs_model.get(index).path)
    }

    Steamworks.SteamInputScope {
        enabled: true

        pressHandlers: {
            "folder_down": view.moveCurrentIndexDown,
            "folder_up": view.moveCurrentIndexUp,
            "folder_left": view.moveCurrentIndexLeft,
            "folder_right": view.moveCurrentIndexRight,
            "folder_activate": () => root.cdIndex(view.currentIndex),
            "folder_go_up": fs_model.goUp,
            "folder_go_home": fs_model.goHome
        }

        analogHandlers: {
            "folder_scroll": e => view.flick(e.y * 50, e.y * 50)
        }

        onAnalogEvent: {
            view.moveCurrentIndexDown()
        }
    }

    Keys.onPressed: {
        event.accepted = true

        switch (event.key) {
        case Qt.Key_Return:
            root.cdIndex(view.currentIndex)
            break
        case Qt.Key_Escape:
            fs_model.goUp()
            break
        case Qt.Key_Home:
            fs_model.goHome()
            break
        case Qt.Key_Up:
            view.moveCurrentIndexUp()
            break
        case Qt.Key_Down:
            view.moveCurrentIndexDown()
            break
        case Qt.Key_Left:
            view.moveCurrentIndexLeft()
            break
        case Qt.Key_Right:
            view.moveCurrentIndexRight()
            break
        }
    }

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

        GridView {
            id: view

            model: fs_model

            Layout.fillWidth: true
            Layout.fillHeight: true

            cellWidth: Math.max(parent.width / 4, 300)
            cellHeight: 70
            clip: true

            boundsBehavior: Flickable.StopAtBounds

            ScrollBar.vertical: ScrollBar {}

            populate: Transition {
                id: _transition

                SequentialAnimation {
                    PropertyAction {
                        property: "opacity"
                        value: 0.0
                    }

                    PauseAnimation {
                        duration: 10 * _transition.ViewTransition.index
                    }

                    NumberAnimation {
                        property: "opacity"
                        from: 0.0
                        to: 1.0
                        duration: 200
                        easing.type: Easing.InOutQuad
                    }
                }
            }

            add: _transition

            delegate: FileDelegate {
                width: view.cellWidth - 5
                height: view.cellHeight - 5

                onClicked: {
                    root.cdIndex(index)
                }
            }

            highlight: Rectangle {
                id: fileHighlight

                color: Material.primary
                visible: root.activeFocus

                readonly property bool belowView: {
                    (-view.contentY + fileHighlight.y) < 0
                }

                readonly property bool aboveView: {
                    (-view.contentY + fileHighlight.y) > view.height - view.cellHeight
                }

                readonly property bool inView: {
                    !belowView && !aboveView
                }

                Connections {
                    target: view

                    function onMovingChanged() {
                        if (!view.moving && !fileHighlight.inView) {
                            const y = fileHighlight.belowView ? view.contentY + view.cellHeight : view.contentY + view.height - view.cellHeight
                            view.currentIndex = view.indexAt(fileHighlight.x, y)
                        }
                    }
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
