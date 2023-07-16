import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import "../../../Components" as Core

ItemDelegate {
    height: lineLabel.height

    property var typeIcons: ({
                                 "command": "arrowLeft",
                                 "result": "arrowRight",
                                 "error": "bug",
                                 "info": "information"
                             })

    RowLayout {
        anchors.fill: parent

        Core.MDIIcon {
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
