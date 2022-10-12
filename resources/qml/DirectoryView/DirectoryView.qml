import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15
import Qt.labs.platform 1.1

import QtQuick.Controls.Material 2.12

import "../MDI" as MDI
import ".." as Core

Item {
    id: root

    signal fileOpened(string url)

    DirectoryModel {
        id: folderModel
        _showHidden: showHiddenSwitch.position === 1.0
    }

    function cdIndex(index) {
        if (folderModel.get(index, "fileIsDir")) {
            folderModel.cdIndex(index)
            return
        }
        root.fileOpened(folderModel.get(index, "fileUrl"))
    }

    function onSteamInputDigitalStatesChanged(states) {
        if(states["folder_down"]) {
            view.moveCurrentIndexDown();
        }
        if(states["folder_up"]) {
            view.moveCurrentIndexUp();
        }

        if(states["folder_left"]) {
            view.moveCurrentIndexLeft();
        }

        if(states["folder_right"]) {
            view.moveCurrentIndexRight();
        }
    }


    Keys.onPressed: {
        event.accepted = true

        switch (event.key) {
        case Qt.Key_Return:
            root.cdIndex(view.currentIndex)

            break
        case Qt.Key_Escape:
            folderModel.goUp()
            break
        case Qt.Key_Home:
            folderModel.goHome()
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
                path: folderModel.path
                Layout.preferredHeight: 40
                Layout.preferredWidth: 500

                onPathSelected: {
                    folderModel.cd(path)
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
                    folderModel.goUp()
                }
                enabled: folderModel.canGoUp
            }

            MDI.Button {
                iconName: "home"
                onClicked: {
                    folderModel.goHome()
                }
            }
        }

        GridView {
            id: view

            Connections {
                target: steam_input
                function onAnalogActionStatesChanged(states) {
                    view.flick(states["folder_scroll"].x * 50, states["folder_scroll"].y * 50)
                }
            }

            model: folderModel
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
                model: folderModel

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
                text: `Total: ${folderModel.count}`
                font.pointSize: 14
                verticalAlignment: Text.AlignVCenter
            }
        }
    }
}
