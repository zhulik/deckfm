import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./MDI" as MDI

Pane {
    id: root

    property alias text: headerLabel.text
    property bool collapsed: false

    height: mainLayout.height

    default property alias content: inner.data

    ColumnLayout {
        id: mainLayout
        width: parent.width

        MouseArea {
            Layout.fillWidth: parent
            height: headerLayout.height

            onClicked: {
                root.collapsed = !root.collapsed
            }

            RowLayout {
                id: headerLayout
                width: parent.width
                spacing: 20

                MDI.Icon {
                    name: "chevronRight"
                    Layout.fillHeight: parent
                    rotation: collapsed ? 0 : 90

                    Behavior on rotation {
                        NumberAnimation { duration: 200 }
                    }
                }

                Label {
                    id: headerLabel
                    font.pixelSize: 24
                    Layout.fillWidth: parent
                }
            }
        }

        Item {
            Layout.fillWidth: parent

            clip: true

            Layout.preferredHeight: collapsed ? 0 : inner.height

            Behavior on Layout.preferredHeight {
                NumberAnimation { duration: 200 }
            }

            ColumnLayout {
                id: inner
                width: parent.width
            }
        }
    }
}
