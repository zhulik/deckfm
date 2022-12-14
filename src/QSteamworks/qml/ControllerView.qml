import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "../../../resources/qml/models" as Models
import "../../../resources/qml/MDI" as MDI

Item {
    id: root

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


                RowLayout {
                    Label {
                        Layout.fillWidth: parent
                        verticalAlignment: Text.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        font.pointSize: 24
                        text: `Name: ${controllersView.currentController.name}`
                    }

                    MDI.Button{
                        iconName: "applicationSettings"
                        height: parent.height
                        onClicked: {
                        steam_input.showBindingPanel(controllersView.currentController.handle)
                    }
                }
            }

                Label {
                    Layout.fillWidth: parent
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                    font.pointSize: 24
                    text: `Handle: ${controllersView.currentController.handle}`
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

                    Repeater {
                        model: steam_input.actionSets

                        TabButton {
                            text: modelData.name
                            onClicked: steam_input.actionSet = modelData.name
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
                                Layout.preferredWidth: stack.width / 2 - 10

                                clip: true

                                model: Models.JSONListModel {
                                    data: steam_input.actionSets[stack.currentIndex].actions
                                }

                                delegate: Item {
                                    width: stack.width
                                    height: 50

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
                                            font.pointSize: 20

                                            text: localizedName
                                            verticalAlignment: Qt.AlignVCenter
                                        }
                                    }
                                }
                            }

//                            Timer {
//                                running: true
//                                interval: 300
//                                repeat: true
//                                onTriggered: {
//                                    logView.log({timestamp: new Date().valueOf(), foo: "Bar"})
//                                }
//                            }

                            Label {
                                id: logLabel
                                Layout.preferredWidth: stack.width / 2 - 10
                                Layout.fillHeight: parent
                            }
                        }
                    }
                }
            }
        }
    }
}
