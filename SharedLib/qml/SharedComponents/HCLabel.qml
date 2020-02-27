import QtQuick 2.9
import QtQuick.Controls 2.4

import StyleDef 1.0

Label {
    id: root

    property int fontPointSize: 0

    font.pointSize: fontPointSize>0 ? fontPointSize : Style.fontPointSize-2

    elide: Text.ElideRight

    verticalAlignment: Text.AlignVCenter
}
