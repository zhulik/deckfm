import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: label.width
    height: label.height

    property string name

    Label {
        id: label
    }

    Connections {
        target: steam_input
        function onActionSetsChanged() {
            label.text = `Action name: ${root.name}, glyphs: ${JSON.stringify(steam_input.action(root.name).glyphs)}`
        }
    }
}
