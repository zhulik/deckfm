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

    ColumnLayout {
        anchors.fill: parent


        TabBar {
            id: bar

            property var currentActionSet: steam_input.iga.actionSets[currentIndex]

            Layout.fillWidth: parent

            Label {
                width: 200
                height: 100
                text: "Action set layers"
            }

            Repeater {
                model: steam_input.iga.actionSets

                TabButton {
                    Layout.fillWidth: parent
                    text: modelData.name

                    MDI.Icon {
                        name: "star"
                        visible: input.actionSet === modelData.name

                        anchors.centerIn: parent
                        anchors.horizontalCenterOffset: -100
                    }
                }
            }
        }

        StackLayout {
            id: stack
            Layout.fillWidth: parent
            Layout.fillHeight: parent

            currentIndex: bar.currentIndex

            Repeater {
                model: steam_input.iga.actionSets

                RowLayout {

                    ActionList {
                        Layout.fillHeight: parent
                        Layout.fillWidth: parent

                        actions: bar.currentActionSet.actions

                        header: Label {
                            text: "Actions"
                        }
                    }

                    ListView {
                        id: layersView
                        Layout.fillHeight: parent
                        Layout.fillWidth: parent

                        property var currentLayer: bar.currentActionSet.layers[layersView.currentIndex]

                        clip: true

                        model: Models.JSONListModel {
                            data: bar.currentActionSet.layers
                        }

                        highlight: Rectangle {
                            color: Material.primary
                            visible: true
                        }

                        delegate: ItemDelegate {
                            width: stack.width
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
    }
}
