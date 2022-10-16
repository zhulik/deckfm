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
                        font.pointSize: 36
                        text: name
                    }

                    Item {
                        Layout.fillWidth: parent
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

                Image {
                    Layout.fillHeight: parent
                    Layout.fillWidth: parent

                    verticalAlignment: Image.AlignVCenter
                    horizontalAlignment: Image.AlignHCenter

                    fillMode: Image.PreserveAspectFit
                    source: controllersView.currentController.image
                }

            }

        }
    }
}
