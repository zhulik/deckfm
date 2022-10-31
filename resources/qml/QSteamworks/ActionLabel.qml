import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    id: root

    property string name

    text: JSON.stringify(steam_input.currentActionSet.actions[root.name])
}
