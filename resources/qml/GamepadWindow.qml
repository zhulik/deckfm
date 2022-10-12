import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "./MDI" as MDI

import "./models" as Models

Dialog {
    id: root
    Material.theme: Material.Dark
    modal: true


    header: ToolBar {
        RowLayout {

            anchors.fill: parent

            ToolButton {
                text: "Gamepads"

                font.pointSize: 24
            }

            Item {
                Layout.fillWidth: parent
            }

            MDI.Button {
                iconName: "applicationSettings"
                onClicked: {
                    steam_input.showBindingPanel()
                }
            }

            ToolSeparator {}

            MDI.Button {
                iconName: "windowClose"
                onClicked: root.close()
            }
        }
    }

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: tabBar
            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignTop

            currentIndex: view.currentIndex

            Repeater {
                id: tabRepeater
                //                model: [
                //                    {
                //                        "name": "Steam",
                //                        "handle": 12345,
                //                        "image": "qrc:/resources/images/controllers/Steam.png",
                //                        "type": 1
                //                    },
                //                    {
                //                        "name": "Steam Deck",
                //                        "handle": 12346,
                //                        "image": "qrc:/resources/images/controllers/Steam Deck.png",
                //                        "type": 2
                //                    }
                //                ]
                model: steam_input.connectedControllers
                TabButton {
                    text: modelData.name
                    width: implicitWidth
                }
            }
        }

        ListView {
            id: view

            Layout.fillWidth: parent
            Layout.fillHeight: parent
            clip: true
            model: tabRepeater.model
            orientation: ListView.Horizontal
            snapMode: ListView.SnapToItem

            highlightRangeMode: ListView.StrictlyEnforceRange

            delegate: Item {
                id: gamepadItem

                height: view.height
                width: view.width

                RowLayout {
                    id: layout
                    visible: tabBar.count > 0

                    anchors.fill: gamepadItem

                    ColumnLayout {
                        Layout.preferredWidth: gamepadItem.width / 3

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
                        Layout.preferredWidth: gamepadItem.width / 3

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
                        Layout.fillHeight: gamepadItem
                        Layout.preferredWidth: gamepadItem.width / 3

                        verticalAlignment: Image.AlignVCenter
                        horizontalAlignment: Image.AlignHCenter

                        fillMode: Image.PreserveAspectFit
                        source: modelData.image
                    }
                }
            }
        }
    }


    Label {
        id: label
        anchors.centerIn: parent
        font.pointSize: 48
        text: "No controllers connected"

        visible: tabBar.count == 0
    }
}
