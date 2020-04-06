import QtQuick 2.9
import QtQuick.Controls 2.4
import QtQuick.Controls.Styles 1.4

import StyleDef 1.0

SpinBox {

    property var model
    property int modelIndex

    signal valueChanged

    font.pointSize: Style.fontPointSize
    font.family: Style.fontFamily

    Connections {
        target: model

        onDataChanged: {
            value = model ? model.data(model.index(modelIndex, 0), 258) : 0

            valueChanged()
        }
    }
}
