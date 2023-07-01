import QtQuick 2.15
import QtQml 2.15

Item {
    id: root

    property bool enabled: true

    property string actionSet

    // List of strings. Layers found by name and applied in the order on appearing
    property var actionSetLayers: []

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

        Binding {
            when: enabled

            target: steam_input.lastController
            property: "activeActionSetLayers"

            value: {
                if (steam_input.lastController) {
                    const layers = actionSetLayers.map((layerName) => {
                                             return steam_input.lastController.actionSet.layerByName(layerName)
                                         })

                    steam_input.lastController.activeActionSetLayers = layers
                } else {
                    ""
                }
            }

            restoreMode: Binding.RestoreBindingOrValue
        }

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
