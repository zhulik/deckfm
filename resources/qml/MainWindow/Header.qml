import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import "../Components" as Core

ToolBar {
    id: root

    signal menuClicked
    signal logoClicked
    signal exitClicked

    RowLayout {
        anchors.fill: parent

        Core.MDIButton {
            iconName: "menu"
            onClicked: root.menuClicked()
        }

        Core.MDIButton {
            text: "DECKFM"
            font.pointSize: 24
            onClicked: root.logoClicked()
        }

        Item {
            Layout.fillWidth: parent
        }

        Core.FPSCounter {}

        ToolSeparator {}

        Core.MDIButton {
            iconName: "bug"
            Layout.alignment: Qt.AlignRight
            onClicked: {
                var http = new XMLHttpRequest()
                http.open("GET", "https://google.com")

                http.onreadystatechange = function () {
                    // Call a function when the state changes.
                    if (http.readyState == 4) {
                        if (http.status == 200) {
                            console.log("ok")
                        } else {
                            console.log("error: " + http.status)
                        }
                    }
                }
                http.send()
            }
        }

        ToolSeparator {}

        Core.MDIButton {
            iconName: "windowClose"
            Layout.alignment: Qt.AlignRight
            onClicked: root.exitClicked()
        }
    }
}
