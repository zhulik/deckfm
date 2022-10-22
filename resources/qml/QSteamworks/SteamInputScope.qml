import QtQuick 2.15
import QtQml 2.15

Item {
    id: root

    property bool enabled: true
    property string actionSet

    property var actionStates: steam_input.actionStates

    property var pressHandlers: ({})
    property var releaseHandlers: ({})
    property var analogHandlers: ({})

    signal inputEvent(var event)

    signal pressedEvent(var event)
    signal releasedEvent(var event)
    signal analogEvent(var event)

    Binding {
        when: enabled && actionSet != ""

        target: steam_input
        property: "actionSet"
        value: actionSet

        restoreMode: Binding.RestoreBindingOrValue
    }

    Connections {
        target: steam_input
        enabled: root.enabled

        function onPressedEvent(event) {
            root.pressedEvent(event)
            sendInputEvent(pressHandlers, event)
        }

        function onReleasedEvent(event) {
            root.releasedEvent(event)
            sendInputEvent(releaseHandlers, event)
        }

        function onAnalogEvent(event) {
            root.analogEvent(event)
            sendInputEvent(analogHandlers, event)
        }

        function onInputEvent(event) {
            root.inputEvent(event)
        }

        function sendInputEvent(handlers, event) {
            const handler = handlers[event.action.actionDefinition.name]

            if (handler) {
                handler(event)
            }
        }
    }
}
