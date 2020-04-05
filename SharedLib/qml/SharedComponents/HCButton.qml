import QtQuick 2.0
import QtQuick.Controls 2.12

import StyleDef 1.0

Button {
    id: root

    font.pointSize: Style.fontPointSize
    font.family: Style.fontFamily

    background: Rectangle {
        color: root.down ? "#d6d6d6" : "#f6f6f6"
        border.color: "#26282a"
        border.width: 1
        radius: 4
    }
}
