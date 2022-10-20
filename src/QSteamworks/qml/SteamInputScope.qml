import QtQuick 2.15

Item {
    id: root
    // TODO: focus tracking and actionSet switching

    property var events: []
    property bool enabled: true
    property var actionStates: ({})

    signal inputEvent(var event)

    Connections {
        target: steam_input
        enabled: root.enabled

        function onInputEvent(event) {
            const states = actionStates

            if (event.action.actionDefinition.isDigital) {
                states[event.action.actionDefinition.name] = event.digitalState
            } else {
                states[event.action.actionDefinition.name] = { x: event.analogX, y: event.analogY }
            }

            actionStates = states

            if (events.includes(event.action.actionDefinition.name) || events.length == 0) {
              root.inputEvent(event)
            }
        }
    }
}
