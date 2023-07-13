import QtQuick 2.15

import Steamworks.SteamInput 1.0 as SI

SI.SteamInputControl {
    enabled: visible
    actionSet: "deckfm"

    controller: steam_input.lastController

    function scrollHaptic() {
        if (!controller) {
            return
        }

        controller.triggerRepeatedHapticPulse(500, 1, 1)
    }

    function stopScrollMomentum() {
        if (!controller) {
            return
        }

        const a = controller.actionSet.actions["scroll"]
        controller.stopAnalogActionMomentum(a)
    }

    function stopScrollHaptic() {
        if (!controller) {
            return
        }

        controller.triggerRepeatedHapticPulse(2900, 1200, 2)
    }
}
