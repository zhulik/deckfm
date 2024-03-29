import QtQuick 2.15
import QtQuick.Controls 2.15

import "icons.js" as MdiFont

Label {
    property string name

    text: if (name) {
              MdiFont.Icon[name]
          } else {
              ""
          }

    font.family: "Material Design Icons"

    horizontalAlignment: Text.AlignHCenter
    verticalAlignment: Text.AlignVCenter

    font.pixelSize: parent.height * 0.6
}
