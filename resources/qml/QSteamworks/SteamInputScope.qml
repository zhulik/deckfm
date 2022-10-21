import QtQuick 2.15

Item {
    id: root

    // TODO: focus tracking and actionSet switching
    property bool enabled: true
    property var actionStates: ({})

    property var pressHandlers: ({})
    property var releaseHandlers: ({})
    property var analogHandlers: ({})

    signal inputEvent(var event)

    signal pressedEvent(var event)
    signal releasedEvent(var event)
    signal analogEvent(var event)

    Connections {
        target: steam_input
        enabled: root.enabled

        function onInputEvent(event) {
            const actionName = event.action.actionDefinition.name

            actionStates[actionName] = getState(event)

            actionStates = actionStates // reassign property so that the changed signal is emitted

            root.inputEvent(event)

            let handlers = analogHandlers
            let signalF = analogEvent

            if (event.action.actionDefinition.isDigital) {
                if (event.digitalState) {
                    handlers = pressHandlers
                    signalF = root.pressedEvent
                } else {
                    handlers = releaseHandlers
                    signalF = root.releasedEvent
                }
            }

            signalF(event)

            const handler = handlers[actionName]

            if (handler) {
                handler(actionStates[actionName])
            }
        }

        function getState(event) {
            if (event.action.actionDefinition.isDigital) {
                return event.digitalState
            } else {
                return {
                    "x": event.analogX,
                    "y": event.analogY
                }
            }
        }
    }
}
