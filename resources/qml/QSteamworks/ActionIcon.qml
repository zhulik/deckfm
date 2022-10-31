import QtQuick 2.15

Item {
    id: root
    width: height

    property string name

    Image {
        anchors.fill: parent

        source: steam_input.action(root.name).glyphs[0];
    }
}
