import QtQuick 2.15
import QtQuick.Controls 2.15

ListView {
    id: root

    property bool atEnd: contentHeight-contentY - height <= height / 3

    currentIndex: atEnd ? count - 1 : currentIndex
    clip: true
    model: ListModel {}

    function log(msg) {
        model.append({
                         "message": msg
                     })
    }

    ScrollBar.vertical: ScrollBar {
        active: true
    }

    delegate: Item {
        height: label.height + 10

        Label {
            id: label

            font.pointSize: 14
            text: JSON.stringify(message, null, 2)
            verticalAlignment: Qt.AlignVCenter
        }
    }
}
