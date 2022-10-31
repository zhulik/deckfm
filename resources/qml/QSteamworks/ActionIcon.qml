import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: height

    property string name

    Image {
        id: image

        anchors.fill: parent
    }

    Connections {
        target: steam_input

        function onActionSetsChanged() {
            image.source = steam_input.action(root.name).glyphs[0];
        }
    }
}
