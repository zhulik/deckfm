import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: height

    property string name

    Label {
        anchors.fill: parent

        text: `Action name: ${root.name}, action: ${JSON.stringify(steam_input.action(root.name))}`
    }
}
