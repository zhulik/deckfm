import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import "./MDI" as MDI

ToolBar {
    id: root

    signal menuClicked
    signal logoClicked
    signal exitClicked

    RowLayout {
        anchors.fill: parent

        MDI.Button {
            iconName: "menu"
            onClicked: root.menuClicked()
        }

        MDI.Button {
            text: "DECKFM"
            font.pointSize: 24
            onClicked: root.logoClicked()
        }

        Item {
            Layout.fillWidth: parent
        }

        FPSCounter {}

        ToolSeparator {}

        MDI.Button {
            iconName: "bug"
            Layout.alignment: Qt.AlignRight
            onClicked: {
                console.log(qmlEngine)
            }
        }

        ToolSeparator {}


        MDI.Button {
            iconName: "windowClose"
            Layout.alignment: Qt.AlignRight
            onClicked: root.exitClicked()
        }
    }
}
