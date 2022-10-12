import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "../models" as Models

Item {
    id: root

    RowLayout {
        id: layout

        anchors.fill: root

        ColumnLayout {
            Layout.preferredWidth: root.width / 3

            Label {
                text: `Name: ${modelData.name}`
            }

            Label {
                text: `Current action set: ${steam_input.actionSet}`
            }

            ListView {
                Layout.fillHeight: parent
                Layout.fillWidth: parent

                clip: true

                model: Models.JSONListModel {
                    id: digitalActionsModel
                    data: steam_input.digitalActions
                }

                delegate: Item {
                    width: parent.width
                    height: 70

                    RowLayout {
                        anchors.fill: parent

                        Image {
                            width: 40
                            height: 40
                            source: `file://${glyphs[0]}`
                        }

                        Label {
                            Layout.fillWidth: parent
                            height: parent.height

                            font.pixelSize: steam_input.digitalActionStates[name] ? 36 : 24
                            text: name
                            verticalAlignment: Qt.AlignVCenter
                        }
                    }
                }
            }
        }

        ColumnLayout {
            Layout.preferredWidth: root.width / 3

            ListView {
                Layout.fillHeight: parent
                Layout.fillWidth: parent
                clip: true

                model: Models.JSONListModel {
                    id: analogActionsModel
                    data: steam_input.analogActions
                }

                delegate: Item {
                    width: parent.width
                    height: 70

                    RowLayout {
                        anchors.fill: parent

                        Image {
                            width: 40
                            height: 40
                            source: `file://${glyphs[0]}`
                        }

                        Label {
                            Layout.fillWidth: parent
                            height: parent.height

                            font.pixelSize: 24
                            text: `${name}: ${steam_input.analogActionStates[name].x}x${steam_input.analogActionStates[name].y}`
                            verticalAlignment: Qt.AlignVCenter
                        }
                    }
                }
            }
        }

        Image {
            Layout.fillHeight: root
            Layout.preferredWidth: root.width / 3

            verticalAlignment: Image.AlignVCenter
            horizontalAlignment: Image.AlignHCenter

            fillMode: Image.PreserveAspectFit
            source: modelData.image
        }
    }
}
