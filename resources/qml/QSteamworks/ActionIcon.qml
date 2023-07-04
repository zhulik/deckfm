import QtQuick 2.15
import QtQuick.Controls 2.15

Image {
    property string name
    width: height

    fillMode: Image.PreserveAspectFit

    readonly property var activeControl: try {
                                             steam_input_scope.activeControls.find(
                                                         x => !x.global)
                                         } catch (e) {
                                             null
                                         }

    source: try {
                activeControl.controller.actionSet.actions[name].glyphs[0] || ""
            } catch (e) {
                ""
            }
}
