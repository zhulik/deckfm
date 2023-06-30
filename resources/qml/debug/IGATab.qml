import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "../../../resources/qml/models" as Models
import "../../../resources/qml/MDI" as MDI

Rectangle {
    id: root

    color: Material.background

    opacity: 1

    TabView {
        id: actionSetsTabView
        anchors.fill: parent

        model: steam_input.iga.actionSets

        tabDelegate: TabButton {
            Layout.fillWidth: parent
            text: modelData.name
        }

        delegate: RowLayout {
            ActionList {
                Layout.fillHeight: parent
                Layout.fillWidth: parent

                actions: actionSetsTabView.selectedItem.actions

                header: Label {
                    text: "Actions"
                }
            }

            ListView {
                id: layersView
                Layout.fillHeight: parent
                Layout.fillWidth: parent

                property var currentLayer: actionSetsTabView.selectedItem.layers[layersView.currentIndex]

                clip: true

                model: Models.JSONListModel {
                    data: actionSetsTabView.selectedItem.layers
                }

                highlight: Rectangle {
                    color: Material.primary
                }

                delegate: ItemDelegate {
                    width: layersView.width
                    height: 50

                    onClicked: {
                        layersView.currentIndex = index
                    }

                    RowLayout {
                        anchors.fill: parent

                        Label {
                            Layout.fillWidth: parent
                            height: parent.height
                            font.pointSize: 20

                            text: model.name
                            verticalAlignment: Qt.AlignVCenter
                        }
                    }
                }

                header: Label {
                    text: "Action set layers"
                }
            }

            ActionList {
                Layout.fillHeight: parent
                Layout.fillWidth: parent

                actions: {
                    if (layersView.currentLayer) {
                        layersView.currentLayer.actions
                    }
                }

                header: Label {
                    text: "Actions of selected layer"
                }
            }

        }
    }
}
