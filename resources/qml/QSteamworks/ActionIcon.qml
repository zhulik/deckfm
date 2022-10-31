import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: label.width
    height: label.height

    property string name

    Timer {
        running: true
        repeat: true
        interval: 1000

        onTriggered: {
            const name = root.name
            root.name = ""
            root.name = name
        }
    }

    Label {
        id: label

        text: `Action name: ${root.name}, glyphs: ${JSON.stringify(steam_input.action(root.name).glyphs)}`
    }
}
