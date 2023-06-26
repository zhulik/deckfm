import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

TextArea {
    id: root

    property var history: []
    property int historyPosition: 0

    signal evalRequested(string line)

    onHistoryPositionChanged: {
        text = historyPosition === history.length ? "" : history[historyPosition]
    }

    Keys.onPressed: (event)=> {
                        switch(event.key) {
                            case Qt.Key_Return:
                            if (event.modifiers === 0) {
                                event.accepted = true
                                root.evalRequested(root.text)
                                history.push(root.text)
                                historyPosition = history.length
                            }
                            break

                            case Qt.Key_Up:
                            if (historyPosition > 0) {
                                event.accepted = true
                                historyPosition -= 1
                                root.cursorPosition = root.text.length
                            }
                            break

                            case Qt.Key_Down:
                            if (historyPosition < history.length) {
                                event.accepted = true
                                historyPosition += 1
                                root.cursorPosition = root.text.length
                            }
                            break
                        }
                    }
}
