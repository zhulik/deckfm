import QtQuick 2.15

Item {
    id: root
    width: height

    property string name

    Image {
        id: image

        anchors.fill: parent
        source: steam_input.action(root.name).glyphs[0];
    }

    Connections {
        target: steam_input

        function onActionSetsChanged() {
            image.source = steam_input.action(root.name).glyphs[0];
        }
    }
}
