import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import QtQuick.Controls.Material 2.12

import "./MDI" as MDI

Dialog {
    id: root
    Material.theme: Material.Dark
    anchors.centerIn: parent
    width: parent.width * 0.98
    height: parent.height * 0.98
    modal: true


    header: ToolBar {
        RowLayout {

            anchors.fill: parent

            ToolButton {
                text: "Gamepad"

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
                id: repeater

                model: steam_input.connectedControllers

                TabButton {
                    text: modelData.name
                    width: implicitWidth
                }
            }
        }


        Item {
            id: controllerView
            Layout.fillWidth: parent

            Layout.fillHeight: parent


            readonly property var controller: repeater.model[tabBar.currentIndex]

            RowLayout {
                visible: tabBar.count > 0

                anchors.fill: parent

                Label {
                    Layout.preferredWidth: parent.width / 2
                    text: `Name: ${controllerView.controller.name}`

                }

                Image {
                    Layout.fillHeight: parent
                    Layout.preferredWidth: parent.width / 2

                    verticalAlignment: Image.AlignVCenter
                    horizontalAlignment: Image.AlignHCenter

                    fillMode: Image.PreserveAspectFit
                    source: controllerView.controller.image
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
