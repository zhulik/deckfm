import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

TextArea {
    id: root

    property var history: []
    property int historyPosition: 0

    signal evalRequested(string line)

    Keys.onPressed: (event)=> {
                        if (event.key === Qt.Key_Return && event.modifiers === 0) {
                            event.accepted = true

                            root.evalRequested(root.text)

                            history.push(root.text)

                            historyPosition = history.length
                            root.text = ""

                        }
                        if (event.key === Qt.Key_Up && historyPosition > 0) {
                            event.accepted = true
                            historyPosition -= 1
                            root.text = history[historyPosition]
                            root.cursorPosition = root.text.length
                        }

                        if (event.key === Qt.Key_Down) {
                            if (historyPosition >= history.length) {
                                return
                            }

                            event.accepted = true
                            historyPosition += 1
                            if (historyPosition == history.length) {
                                root.text = ""
                            } else  {
                                root.text = history[historyPosition]
                                root.cursorPosition = root.text.length
                            }
                        }
                    }
}
