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

    RowLayout {
        anchors.fill: parent

        ListView {
            id: controllersView

            property var selectedController: model[currentIndex]

            Layout.preferredWidth: root.width / 3
            Layout.fillHeight: parent

            model: steam_input.controllers

            delegate: ItemDelegate {
                width: controllersView.width
                height: 70

                onClicked: controllersView.currentIndex = index

                RowLayout {
                    anchors.fill: parent

                    Image {
                        Layout.fillHeight: parent
                        Layout.maximumWidth: 100

                        verticalAlignment: Image.AlignVCenter
                        horizontalAlignment: Image.AlignHCenter

                        fillMode: Image.PreserveAspectFit
                        source: modelData.image
                    }

                    Label {
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 24
                        text: modelData.name
                    }

                    Item {
                        Layout.fillWidth: parent
                    }
                }
            }

            highlight: Rectangle {
                color: Material.primary
            }
        }

        ColumnLayout {
            Layout.preferredWidth: root.width / 3 * 2
            Layout.fillHeight: parent
            visible: !!controllersView.selectedController

            RowLayout {
                Label {
                    Layout.fillWidth: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 24
                    text: if (controllersView.selectedController) {
                              `Name: ${controllersView.selectedController.name}`
                          } else {
                              ""
                          }
                }

                MDI.Button{
                    iconName: "applicationSettings"
                    height: parent.height
                    onClicked: {
                        steam_input.showBindingPanel(controllersView.selectedController.handle)
                    }
                }
            }

            Label {
                Layout.fillWidth: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 24
                text: if (controllersView.selectedController) {
                          `Handle: ${controllersView.selectedController.handle}`
                      } else {
                          ""
                      }
            }

            Label {
                Layout.fillWidth: parent
                verticalAlignment: Text.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                font.pointSize: 24
                text: `ActionSet: ${steam_input.actionSet}`
            }

            TabBar {
                id: bar

                Layout.fillWidth: parent

                property var currentActionSet: if (controllersView.selectedController) {
                                                   controllersView.selectedController.actionSets[currentIndex]
                                               } else {
                                                   undefined
                                               }

                Repeater {
                    model: if (controllersView.selectedController) {
                               controllersView.selectedController.actionSets
                            } else {
                               []
                           }

                    TabButton {
                        text: modelData.name
                    }
                }
            }

            StackLayout {
                id: stack
                Layout.fillWidth: parent
                Layout.fillHeight: parent

                currentIndex: bar.currentIndex

                Repeater {
                    model: if (controllersView.selectedController) {
                               controllersView.selectedController.actionSets
                            } else {
                               []
                           }

                    RowLayout {
                        Layout.fillHeight: parent
                        Layout.fillWidth: parent

                        ListView {
                            Layout.fillHeight: parent
                            Layout.preferredWidth: stack.width / 2 - 10

                            clip: true

                            model: Models.JSONListModel {
                                data: if (bar.currentActionSet) {
                                          bar.currentActionSet.actions
                                      } else {
                                          []
                                      }
                            }

                            delegate: Item {
                                width: stack.width
                                height: 50

                                RowLayout {
                                    anchors.fill: parent

                                    Image {
                                        width: height
                                        height: parent.height
                                        source: if (glyphs[0]) {
                                                    `file://${glyphs[0]}`
                                                } else {
                                                    ""
                                                }
                                    }

                                    Label {
                                        Layout.fillWidth: parent
                                        height: parent.height
                                        font.pointSize: 20

                                        text: localizedName
                                        verticalAlignment: Qt.AlignVCenter
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
    }
}
