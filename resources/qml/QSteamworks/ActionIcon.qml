import QtQuick 2.15
import QtQuick.Controls 2.15

Image {
    width: height

    property string name

    source: steam_input.currentActionSet.actions[name].glyphs[0]
}
