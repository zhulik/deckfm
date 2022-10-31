import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: height

    property string name

    Image {
        id: image

        anchors.fill: parent
        source: steam_input.action(root.name).glyphs[0];
    }

    Label {
        text: JSON.stringify(steam_input.action(root.name).glyphs)
    }

    Connections {
        target: steam_input

        function onActionSetsChanged() {
            image.source = steam_input.action(root.name).glyphs[0];
        }
    }
}
