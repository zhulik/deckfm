import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    property string name

    readonly property var activeControl: try {
                                             steam_input_scope.activeControls.find(
                                                         x => !x.global)
                                         } catch (e) {
                                             null
                                         }

    text: try {
              steam_input.lastController.actionSet.actions[name].localizedName
          } catch (e) {
              ""
          }
}
