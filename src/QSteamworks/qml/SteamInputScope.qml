import QtQuick 2.15

Item {
    id: root
    // TODO: focus tracking and actionSet switching

    property bool enabled: true
    property var actionStates: ({})
    property var handlers: ({})

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

            const handler = handlers[event.action.actionDefinition.name]

            if (handler) {
                handler(event)
            }
        }
    }
}
