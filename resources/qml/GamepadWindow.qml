import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "./MDI" as MDI

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
                Layout.fillWidth: parent
                Layout.maximumWidth: 200
            }

            Item {
                Layout.fillWidth: parent
            }

            ToolSeparator {}

            MDI.Button {
                iconName: "windowClose"
                Layout.alignment: Qt.AlignRight
                onClicked: root.close()
            }
        }
    }
    footer: ToolBar {}

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: tabBar
            Layout.preferredWidth: parent.width
            Layout.alignment: Qt.AlignTop

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
            snapMode: ListView.SnapOneItem
            highlightRangeMode: ListView.StrictlyEnforceRange


            preferredHighlightBegin: 0
            preferredHighlightEnd: 10

            onCurrentIndexChanged: {
                tabBar.currentIndex = currentIndex
            }


            delegate: Item {
                id: gamepadItem

                height: view.height
                width: view.width

                RowLayout {
                    id: layout
                    visible: tabBar.count > 0

                    anchors.fill: gamepadItem

                    ColumnLayout {
                        Layout.preferredWidth: gamepadItem.width / 2

                        Label {
                            text: `Name: ${modelData.name}`
                        }

                        Label {
                            id: menuSelectState

                            Connections {
                                    target: steam_input

                                    function onDigitalActionActivated(name, active) {
                                        menuSelectState.text = `${name}`
                                    }
                                }
                        }
                    }

                    Image {
                        Layout.fillHeight: gamepadItem
                        Layout.preferredWidth: gamepadItem.width / 2

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
