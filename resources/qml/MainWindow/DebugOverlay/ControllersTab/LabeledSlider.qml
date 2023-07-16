import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

RowLayout {
    id: root

    property alias from: slider.from
    property alias to: slider.to
    property alias value: slider.value
    property alias stepSize: slider.stepSize

    Slider {
        id: slider
        Layout.fillWidth: parent
    }

    Label {
        text: slider.value
    }
}
