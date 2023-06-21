import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import "./ConsoleComponents"

Item {
    id: root

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: replView

            Layout.fillHeight: parent
            Layout.fillWidth: parent

            verticalLayoutDirection: ListView.BottomToTop

            model: ListModel {
                id: logsModel

                ListElement {
                    replType: "info"
                    replLine: ">>>"
                }

                function addLine(type, line) {
                    insert(0, {replLine: line, replType: type})
                }
            }

            delegate: LineDelegate {
                width: parent.width
            }
        }

        ReplEdit {
            id: replEdit
            Layout.fillWidth: parent

            onEvalRequested: {
                logsModel.addLine("command", line)
                try {
                    logsModel.addLine("result", `${eval(line)}`)
                } catch (e) {
                    logsModel.addLine("error", `${e}`)
                }
            }
        }
    }
}
