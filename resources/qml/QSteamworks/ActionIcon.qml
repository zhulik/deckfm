import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: root
    width: height

    property string name

    Label {
        anchors.fill: parent

        text: `Glyph path: ${steam_input.action(root.name).glyphs[0]}`
    }
}
