import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls.Material 2.12

import "../../../../resources/qml/MDI" as MDI

ListView {
    id: root

    property var selectedController: model[currentIndex]

    model: steam_input.controllers

    delegate: ItemDelegate {
        width: root.width
        height: 140

        onClicked: root.currentIndex = index

        RowLayout {
            anchors.fill: parent

            Image {
                Layout.fillHeight: parent
                Layout.maximumWidth: parent.height

                fillMode: Image.PreserveAspectFit
                source: modelData.image
            }

            ColumnLayout {
                Layout.fillWidth: parent

                Label {
                    height: parent.height
                    font.pointSize: 24
                    text: modelData.name
                }

                Label {
                    Layout.fillWidth: parent
                    text: `Handle: ${modelData.handle}`
                }

                Label {
                    text: `Action set: ${modelData.actionSet.name}`
                }
            }

            MDI.Icon {
                name: "star"

                font.pixelSize: 30
                visible: modelData === steam_input.lastController
            }

            MDI.Button{
                iconName: "applicationSettings"
                height: parent.height

                onClicked: {
                    root.selectedController.showBindingPanel()
                }
            }
        }
    }

    highlight: Rectangle {
        color: Material.primary
    }
}
