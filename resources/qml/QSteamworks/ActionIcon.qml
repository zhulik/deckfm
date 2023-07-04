import QtQuick 2.15
import QtQuick.Controls 2.15

Image {
    property string name
    width: height

    fillMode: Image.PreserveAspectFit
    source: if (steam_input.lastController
                    && steam_input.lastController.actionSet
                    && steam_input.lastController.actionSet.actions[name]) {
                steam_input.lastController.actionSet.actions[name].glyphs[0]
                        || ""
            } else {
                ""
            }
}
