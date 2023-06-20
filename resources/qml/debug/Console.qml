import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

Item {
    id: root
    signal closed()

    property var deckfmSettings: ({
                                      showCloseButton: false,
                                      showFooter: false
                                  })

    property var labels:({// TODO: MDI
                             "command": ">>",
                             "result": "<-",
                             "error": "X>",
                             "info": "=="
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
            }

            delegate: ItemDelegate {
                width: replView.width
                height: lineLabel.height

                RowLayout {
                    anchors.fill: parent

                    Label {
                        Layout.preferredWidth: 20
                        text: root.labels[replType]
                    }

                    Label {
                        id: lineLabel
                        Layout.fillWidth: parent

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
                logsModel.insert(0, {replLine: cmd, replType: "command"})
                try {
                    logsModel.insert(0, {replLine: `${eval(cmd)}`, replType: "result"})
                } catch (e) {
                    logsModel.insert(0, {replLine: `${e}`, replType: "error"})
                }
            }
        }
    }

    Timer {
        interval: 100
        repeat: false
        running: true

        onTriggered: {
            replEdit.forceActiveFocus()
            logsModel.insert(0, {replLine: ">>>", replType: "info"})
        }
    }
}
