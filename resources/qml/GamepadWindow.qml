import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

import "./MDI" as MDI
import "./models" as Models
import "./GamepadsInfo" as GI

Dialog {
    id: root

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

    GI.GamepadsInfo {}
}
