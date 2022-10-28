import QtQuick 2.15
import QtQuick.Layouts 1.15

import "../../MDI" as MDI

RowLayout {
    id: root

    property bool playing: false
    property bool hasPrevious: false
    property bool hasNext: false

    signal previousPressed()
    signal playPausePressed()
    signal nextPressed()

    MDI.Button {
        Layout.fillHeight: parent
        iconName: "skipPrevious"
        visible: hasPrevious
        onPressed: root.previousPressed()
    }

    MDI.Button {
        Layout.fillHeight: parent
        iconName: playing ? "pause" : "play"
        onPressed: root.playPausePressed()
    }

    MDI.Button {
        Layout.fillHeight: parent
        iconName: "skipNext"
        visible: hasNext
        onPressed: root.nextPressed()
    }
}
