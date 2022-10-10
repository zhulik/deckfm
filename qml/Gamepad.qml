import QtQuick 2.15

import QtGamepad 1.15 as GP

Item {
    id: root

    readonly property bool analogMove: {
        [gamepad.axisLeftX, gamepad.axisLeftY, gamepad.axisRightX, gamepad.axisRightY, buttonL2, buttonR2].map(Math.abs).reduce((a, b) => a + b, 0)
    }

    default property var repeatButtons: new Array

    readonly property bool buttonL2Switch: gamepad.buttonL2 > 0.9
    readonly property bool buttonR2Switch: gamepad.buttonR2 > 0.9

    readonly property var keyMapping: {
        let mapping = {}

        mapping[Gamepad.A] = Qt.Key_Return
        mapping[Gamepad.B] = Qt.Key_Escape
        mapping[Gamepad.X] = Qt.Key_End
        mapping[Gamepad.Y] = Qt.Key_Home
        mapping[Gamepad.Start] = Qt.Key_F1
        mapping[Gamepad.Select] = Qt.Key_F2
        mapping[Gamepad.Up] = Qt.Key_Up
        mapping[Gamepad.Down] = Qt.Key_Down
        mapping[Gamepad.Left] = Qt.Key_Left
        mapping[Gamepad.Right] = Qt.Key_Right
        mapping[Gamepad.L1] = Qt.Key_F3
        mapping[Gamepad.R2] = Qt.Key_F4

        mapping
    }

    readonly property point leftStick: {
        Qt.point(gamepad.axisLeftX, gamepad.axisLeftY)
    }

    readonly property point rightStick: {
        Qt.point(gamepad.axisRightX, gamepad.axisRightY)
    }

    property alias buttonL2: gamepad.buttonL2
    property alias buttonR2: gamepad.buttonR2
    property alias buttonAny: gamepad.buttonAny

    signal buttonPressed(int button)
    signal buttonReleased(int button)

    enum Button {
        A = 1024,
        B,
        X,
        Y,
        Up,
        Down,
        Left,
        Right,
        L1,
        L2Switch,
        R1,
        R2Switch,
        Select,
        Start
    }

    Timer {
        interval: 33
        repeat: true
        triggeredOnStart: true

        running: analogMove

        onTriggered: {

            root.leftStickChanged(Qt.point(gamepad.axisLeftX,
                                           gamepad.axisLeftY))
            root.leftStickChanged(Qt.point(gamepad.axisRightX,
                                           gamepad.axisRightY))

            root.buttonL2Changed()
            root.buttonR2Changed()
        }
    }

    function handleButtonState(value, button) {
        return value ? root.buttonPressed(button) : root.buttonReleased(button)
    }

    onButtonL2SwitchChanged: handleButtonState(buttonL2Switch, Gamepad.L2Switch)
    onButtonR2SwitchChanged: handleButtonState(buttonR2Switch, Gamepad.R2Switch)

    GP.Gamepad {
        id: gamepad

        readonly property bool buttonAny: {
            [buttonA, buttonB, buttonX, buttonY, buttonUp, buttonDown, buttonLeft, buttonRight, buttonSelect, buttonStart, buttonL1, buttonR1].reduce((a, b) => a || b, false)
        }

        deviceId: GP.GamepadManager.connectedGamepads.length
                  > 0 ? GP.GamepadManager.connectedGamepads[0] : -1

        onButtonAChanged: handleButtonState(value, Gamepad.A)
        onButtonBChanged: handleButtonState(value, Gamepad.B)
        onButtonXChanged: handleButtonState(value, Gamepad.X)
        onButtonYChanged: handleButtonState(value, Gamepad.Y)
        onButtonUpChanged: handleButtonState(value, Gamepad.Up)
        onButtonDownChanged: handleButtonState(value, Gamepad.Down)
        onButtonLeftChanged: handleButtonState(value, Gamepad.Left)
        onButtonRightChanged: handleButtonState(value, Gamepad.Right)
        onButtonSelectChanged: handleButtonState(value, Gamepad.Select)
        onButtonStartChanged: handleButtonState(value, Gamepad.Start)
        onButtonL1Changed: handleButtonState(value, Gamepad.L1)
        onButtonR1Changed: handleButtonState(value, Gamepad.R1)
    }

    onButtonPressed: {
        const mapped = keyMapping[button]

        if (mapped) {
            gamepad_bridge.send_key_press(mapped)
        }
    }
}
