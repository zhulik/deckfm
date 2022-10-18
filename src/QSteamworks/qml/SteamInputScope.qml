import QtQuick 2.15

Item {
    id: root
    // TODO: focus tracking and actionSet switching

    property var events: []

    signal inputEvent(var event)

    Connections {
        target: steam_input

        function onInputEvent(event) {
            if (events.includes(event.action.actionDefinition.name) || events.length == 0) {
              root.inputEvent(event)
            }
        }
    }
}
