import QtQuick 2.15
import QtQuick.Layouts 1.15
import QtQuick.Controls 2.15

import QtQuick.Controls.Material 2.12

GridView {
    id: root

    property alias actionSetLayers: inputControl.actionSetLayers

    property var pressHandlers: {

    }

    readonly property var navigationActions: (new Map([[Qt.Key_Up, root.moveCurrentIndexUp], [Qt.Key_Down, root.moveCurrentIndexDown], [Qt.Key_Left, root.moveCurrentIndexLeft], [Qt.Key_Right, root.moveCurrentIndexRight]]))

    clip: true
    keyNavigationWraps: false

    boundsBehavior: Flickable.StopAtBounds

    ScrollBar.vertical: ScrollBar {}

    Keys.onPressed: {
        if (navigationActions.has(event.key)) {
            event.accepted = true
            navigationActions.get(event.key)()
        }
    }

    highlight: Rectangle {
        id: hightlight

        color: Material.primary
        visible: root.activeFocus

        readonly property bool belowView: {
            (-root.contentY + hightlight.y) < 0
        }

        readonly property bool aboveView: {
            (-root.contentY + hightlight.y) > root.height - root.cellHeight
        }

        readonly property bool inView: {
            !belowView && !aboveView
        }

        Connections {
            target: root

            function onMovingChanged() {
                if (!root.moving && !hightlight.inView) {
                    const y = hightlight.belowView ? root.contentY + root.cellHeight : root.contentY
                                                     + root.height - root.cellHeight
                    root.currentIndex = root.indexAt(hightlight.x, y)
                }
            }
        }
    }

    populate: Transition {
        id: _transition

        SequentialAnimation {
            PropertyAction {
                property: "opacity"
                value: 0.0
            }

            PauseAnimation {
                duration: 10 * _transition.ViewTransition.index
            }

            NumberAnimation {
                property: "opacity"
                from: 0.0
                to: 1.0
                duration: 200
                easing.type: Easing.InOutQuad
            }
        }
    }

    add: _transition

    SteamInputControl {
        id: inputControl

        objectName: "GridView"

        pressHandlers: Object.assign({
                                         "down": root.moveCurrentIndexDown,
                                         "up": root.moveCurrentIndexUp,
                                         "left": root.moveCurrentIndexLeft,
                                         "right": root.moveCurrentIndexRight
                                     }, root.pressHandlers)

        property double scrollPos: 0

        analogHandlers: {
            "scroll": e => {
                if ((e.analogY <= 0 && root.atYEnd) || (e.analogY >= 0
                                                        && root.atYBeginning)) {
                    inputControl.stopScrollMomentum()
                    return
                }

                if (e.analogY >= 0 && root.atYBeginning) {
                    inputControl.stopScrollMomentum()
                    return
                }

                scrollPos += Math.abs(e.analogY)

                if (scrollPos >= root.cellHeight) {
                    scrollPos = 0
                    inputControl.scrollHaptic()
                }
                root.contentY -= e.analogY
                root.returnToBounds()
            }
        }
    }

    onCurrentIndexChanged: inputControl.scrollHaptic()

    onAtYBeginningChanged: {
        if (atYBeginning) {
            inputControl.stopScrollMomentum()
            inputControl.stopScrollHaptic()
        }
    }

    onAtYEndChanged: {
        if (atYEnd) {
            inputControl.stopScrollMomentum()
            inputControl.stopScrollHaptic()
        }
    }
}
