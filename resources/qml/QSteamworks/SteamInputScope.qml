import QtQuick 2.15
import QtQml 2.15

Item {
    id: root

    property bool enabled: true

    // TODO empty actionStates if disabled, and rebind if enabled
    property string actionSet
    //    property string actionSetLayer: steam_input.actionSetLayer
    property var actionStates: {
        if (steam_input.lastController) {
            steam_input.lastController.actionStates
        } else {
            undefined
        }
    }

    property var pressHandlers: ({})
    property var releaseHandlers: ({})
    property var analogHandlers: ({})

    signal inputEvent(var event)
    signal pressedEvent(var event)
    signal releasedEvent(var event)
    signal analogEvent(var event)

    Binding {
        id: actionSetBinding
        when: enabled && root.actionSet !== ""

        target: steam_input.lastController
        property: "actionSet"
        value: {
            if (steam_input.lastController) {
                const v = steam_input.lastController.actionSetByName(root.actionSet)
            } else {
                ""
            }
        }

        restoreMode: Binding.RestoreBindingOrValue
    }

    //    Binding {
    //        when: enabled && actionSetLayer !== ""

    //        target: steam_input.lastController
    //        property: "actionSetLayer"
    //        value: actionSetLayer

    //        restoreMode: Binding.RestoreNone
    //    }

    Connections {
        target: steam_input.lastController
        enabled: root.enabled

        function onActionSetsChanged() {
            actionSetBinding.value = steam_input.lastController.actionSetByName(root.actionSet)
        }

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
