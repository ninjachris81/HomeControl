import QtQuick 2.9
import QtQuick.Controls 2.4

import StyleDef 1.0

SpinBox {

    property var model
    property int modelIndex

    signal valueChanged

    Connections {
        target: model

        onDataChanged: {
            value = model ? model.data(model.index(modelIndex, 0), 258) : 0

            valueChanged()
        }
    }

}
