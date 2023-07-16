import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "../../../resources/qml/models" as Models
import "../../../resources/qml/MDI" as MDI

import "./ControllersTabComponents" as Components
import ".." as Core

Rectangle {
    id: root

    color: Material.background

    opacity: 1

    RowLayout {
        anchors.fill: parent

        Components.ControllersView {
            id: controllersView

            Layout.preferredWidth: root.width / 3
            Layout.fillHeight: parent
        }

        ColumnLayout {
            Layout.fillHeight: parent

            Components.HapticsSpoiler {
                Layout.fillWidth: parent

                visible: try {
                             controllersView.selectedController.name === "Steam"
                         } catch (e) {
                             false
                         }

                controller: controllersView.selectedController
            }

            Core.TabView {
                id: actionSetsTabView
                Layout.preferredWidth: root.width / 3 * 2
                Layout.fillHeight: parent
                visible: !!controllersView.selectedController

                model: if (controllersView.selectedController) {
                           controllersView.selectedController.actionSets
                       } else {
                           []
                       }

                tabDelegate: TabButton {
                    text: modelData.name

                    onDoubleClicked: controllersView.selectedController.actionSet
                                     = actionSetsTabView.selectedItem
                }

                delegate: RowLayout {
                    ListView {
                        Layout.fillHeight: parent
                        Layout.fillWidth: parent

                        clip: true

                        model: Models.JSONListModel {
                            data: if (actionSetsTabView.selectedItem) {
                                      actionSetsTabView.selectedItem.actions
                                  } else {
                                      []
                                  }
                        }

                        header: Label {
                            text: "Actions"
                            font.pixelSize: 30
                            font.bold: true
                        }

                        delegate: ItemDelegate {
                            width: actionSetsTabView.width
                            height: 50

                            RowLayout {
                                anchors.fill: parent

                                Image {
                                    width: height
                                    height: parent.height
                                    scale: {
                                        if (!digital) {
                                            return 1
                                        }

                                        const r = controllersView.selectedController.actionStates[name]
                                        if (typeof r === 'undefined') {
                                            return 1
                                        } else {
                                            r ? 0.8 : 1
                                        }
                                    }

                                    source: if (glyphs[0]) {
                                                `file://${glyphs[0]}`
                                            } else {
                                                ""
                                            }
                                }

                                Label {
                                    height: parent.height
                                    font.pointSize: 20

                                    text: localizedName
                                    verticalAlignment: Qt.AlignVCenter
                                }

                                Label {
                                    Layout.fillWidth: parent
                                    height: parent.height

                                    text: {
                                        if (digital) {
                                            return ""
                                        }

                                        const r = controllersView.selectedController.actionStates[name]
                                        typeof r === 'undefined' ? "" : `x: ${r.x}\ny: ${r.y}`
                                    }
                                }
                            }
                        }
                    }

                    ListView {
                        id: layersView
                        Layout.fillHeight: parent
                        Layout.fillWidth: parent

                        clip: true

                        model: if (actionSetsTabView.selectedItem) {
                                   actionSetsTabView.selectedItem.layers
                               } else {
                                   []
                               }

                        header: Label {
                            text: "Actions set layers"
                            font.pixelSize: 30
                            font.bold: true
                        }

                        delegate: ItemDelegate {
                            width: layersView.width
                            height: 50

                            onDoubleClicked: if (controllersView.selectedController.activeActionSetLayers.findIndex(
                                                         x => x.handle === modelData.handle) >= 0) {
                                                 controllersView.selectedController.deactivateActionSetLayer(
                                                             modelData)
                                             } else {
                                                 controllersView.selectedController.activateActionSetLayer(
                                                             modelData)
                                             }

                            RowLayout {
                                anchors.fill: parent

                                Label {
                                    Layout.preferredHeight: parent.height
                                    verticalAlignment: Qt.AlignVCenter
                                    font.pointSize: 20

                                    text: modelData.name
                                }

                                Item {
                                    Layout.fillWidth: parent
                                }

                                Label {
                                    Layout.preferredHeight: parent.height
                                    Layout.preferredWidth: parent.height
                                    verticalAlignment: Qt.AlignVCenter

                                    font.pointSize: 15

                                    text: controllersView.selectedController.activeActionSetLayers.findIndex(
                                              x => x.handle === modelData.handle)

                                    visible: text != "-1"
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
