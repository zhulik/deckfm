import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import "../../MDI" as MDI

ItemDelegate {
    height: lineLabel.height

    property var typeIcons:({
                                "command": "arrowLeft",
                                "result": "arrowRight",
                                "error": "bug",
                                "info": "information"
                            })

    RowLayout {
        anchors.fill: parent

        MDI.Icon {
            Layout.preferredWidth: 30

            name: typeIcons[replType]
            color: "gray"
        }

        Label {
            id: lineLabel

            Layout.fillWidth: parent

            font.pointSize: 16

            text: replLine
        }
    }
}
