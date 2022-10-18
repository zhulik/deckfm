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
            id: actionSetsView

            Layout.preferredWidth: root.width / 3
            Layout.fillHeight: parent

            property string currentActionSet: model.get(currentIndex).value

            model: Models.JSONListModel {
                data: steam_input.iga.actionSets
            }

            delegate: ItemDelegate {
                width: actionSetsView.width
                height: 70

                Label {
                    height: parent.height
                    verticalAlignment: Text.AlignVCenter
                    font.pointSize: 36
                    text: value
                }
                onClicked: actionSetsView.currentIndex = index
            }

            highlight: Rectangle {
                width: actionSetsView.delegate.width
                height: actionSetsView.delegate.height
                color: Material.primary
            }
        }

        ListView {
            id: actionsView

            Layout.preferredWidth: root.width / 3 * 2
            Layout.fillHeight: parent

            model: Models.JSONListModel {
                data: steam_input.iga.qmlActionsForSet(actionSetsView.currentActionSet)
            }

            delegate: ItemDelegate {
                width: actionsView.width
                height: 70

                RowLayout {
                    anchors.fill: parent

                    MDI.Icon {
                        name: isDigital ? "gestureTapButton" : "sineWave"
                        Layout.preferredWidth: 40
                    }

                    Label {
                        Layout.fillHeight: parent

                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 36

                        text: name
                    }

                    Item {
                        Layout.fillWidth: parent
                    }

                    Label {
                        Layout.fillHeight: parent

                        verticalAlignment: Text.AlignVCenter
                        font.pointSize: 36

                        text: type
                    }
                }
            }

            highlight: Rectangle {
                width: actionsView.delegate.width
                height: actionsView.delegate.height
                color: Material.primary
            }
        }
    }
}
