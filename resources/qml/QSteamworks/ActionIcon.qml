import QtQuick 2.15
import QtQuick.Controls 2.15

Image {
    property string name
    width: height


    fillMode: Image.PreserveAspectFit
    source: steam_input.lastController.actionSet.actions[name].glyphs[0]
}
