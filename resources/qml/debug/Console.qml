import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import "../MDI" as MDI

Item {
    id: root
    signal closed()

    property var deckfmSettings: ({
                                      showCloseButton: false,
                                      showFooter: false
                                  })

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
            }

            delegate: ItemDelegate {
                width: replView.width
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
        }

        TextArea {
            id: replEdit
            Layout.fillWidth: parent

            property var history: []

            property int historyPosition: 0

            Keys.onPressed: (event)=> {
                                if (event.key === Qt.Key_Return && event.modifiers === 0) {
                                    event.accepted = true

                                    evalCommand(replEdit.text)

                                    history.push(replEdit.text)

                                    historyPosition = history.length
                                    replEdit.text = ""

                                }
                                if (event.key === Qt.Key_Up && historyPosition > 0) {
                                    event.accepted = true
                                    historyPosition -= 1
                                    replEdit.text = history[historyPosition]
                                    replEdit.cursorPosition = replEdit.text.length
                                }

                                if (event.key === Qt.Key_Down) {
                                    if (historyPosition >= history.length) {
                                        return
                                    }

                                    event.accepted = true
                                    historyPosition += 1
                                    if (historyPosition == history.length) {
                                        replEdit.text = ""
                                    } else  {
                                        replEdit.text = history[historyPosition]
                                        replEdit.cursorPosition = replEdit.text.length
                                    }
                                }
                            }

            function evalCommand(cmd) {
                addLine("command", cmd)
                try {
                    addLine("result", `${eval(cmd)}`)
                } catch (e) {
                    addLine("error", `${e}`)
                }
            }
        }
    }

    function addLine(type, line) {
        logsModel.insert(0, {replLine: line, replType: type})
    }
}
