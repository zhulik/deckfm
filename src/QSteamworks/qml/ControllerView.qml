import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "../../../resources/qml/models" as Models
import "../../../resources/qml/MDI" as MDI

Item {
    id: root

    Connections {
        target: steam_input

        function onDigitalAction() {
            logView.log("Analog action")
        }

        function onAnalogAction() {
            logView.log("Digital action")
        }
    }

    RowLayout {
        anchors.fill: parent

        ListView {
            id: controllersView

            property var currentController: model.get(currentIndex)

            Layout.preferredWidth: root.width / 3
            Layout.fillHeight: parent

            model: Models.JSONListModel {
                data: steam_input.controllers
            }

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
                        source: image
                    }

                    Label {
                        height: parent.height
                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 24
                        text: name
                    }

                    MDI.Button{
                        iconName: "applicationSettings"
                        height: parent.height
                        onClicked: {
                            steam_input.showBindingPanel(handle)
                        }
                    }

                    Item {
                        Layout.fillWidth: parent
                    }

                    MDI.Icon {
                        name: "star"
                        height: parent.height
                        visible: handle === steam_input.currentController.handle
                    }
                }
            }

            highlight: Rectangle {
                width: controllersView.delegate.width
                height: controllersView.delegate.height
                color: Material.primary
            }
        }

        Item {
            Layout.preferredWidth: root.width / 3 * 2
            Layout.fillHeight: parent

            ColumnLayout {
                anchors.fill: parent

                Label {
                    Layout.fillWidth: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 24
                    text: `Name: ${controllersView.currentController.name}`
                }

                Label {
                    Layout.fillWidth: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 24
                    text: `Handle: ${controllersView.currentController.handle}`
                }

                TabBar {
                    id: bar

                    Layout.fillWidth: parent

                    Repeater {
                        model: steam_input.actionSets

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
                        model: steam_input.actionSets

                        RowLayout {
                            Layout.fillHeight: parent
                            Layout.fillWidth: parent

                            ListView {
                                Layout.fillHeight: parent
                                Layout.preferredWidth: stack.width / 2

                                clip: true

                                model: Models.JSONListModel {
                                    data: steam_input.actionSets[stack.currentIndex].actions
                                }

                                delegate: Item {
                                    width: stack.width
                                    height: 210

                                    RowLayout {
                                        anchors.fill: parent

                                        Image {
                                            width: height
                                            height: parent.height
                                            source: `file://${glyphs[0]}`
                                        }

                                        Label {
                                            Layout.fillWidth: parent
                                            height: parent.height

                                            //                                        font.pixelSize: steam_input.digitalActionStates[name] ? 36 : 24
                                            text: JSON.stringify(steam_input.actionSets[stack.currentIndex].actions[index], null,2)
                                            verticalAlignment: Qt.AlignVCenter
                                        }
                                    }
                                }
                            }

                            ListView {
                                id: logView
                                Layout.fillHeight: parent
                                Layout.preferredWidth: stack.width / 2

                                function log(msg) {
                                    logModel.append({msg: msg})
                                }

                                clip: true

                                model: Models.JSONListModel {
                                    id: logModel
                                    data: []
                                }

                                delegate: Item {
                                    width: parent.width
                                    height: 70

                                    Label {
                                        Layout.fillWidth: parent
                                        height: parent.height

                                        text: JSON.stringify(modelData)
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
