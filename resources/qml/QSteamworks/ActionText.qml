import QtQuick 2.15
import QtQuick.Controls 2.15

Label {
    property string name

    text: steam_input.lastController.actionSet.actions[name].localizedName
}
