import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    id: root

    property string name

    Connections {
        target: steam_input

        function onActionSetsChanged() {
            root.text = JSON.stringify(steam_input.action(root.name))
        }
    }
}
