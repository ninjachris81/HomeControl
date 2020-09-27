import QtQuick 2.0
import QtQuick.Controls 2.4
import QtQuick.Layouts 1.3
import QtQuick.VirtualKeyboard 2.1

HCSimpleLabel {

    property var model
    property int modelIndex
    property var conversionFunction: function(v) {return v}

    signal valueChanged

    Connections {
        target: model

        onDataChanged: {
            labelValue = conversionFunction(model ? model.data(model.index(modelIndex, 0), 258) : "")
            isValid = model ? model.data(model.index(modelIndex, 0), 259) : false
            trend = model ? model.data(model.index(modelIndex, 0), 260) : 0

            valueChanged()
        }
    }

    onModelChanged: {
        labelText = qsTranslate("Constants", model.data(model.index(modelIndex, 0), 257))
    }

    onModelIndexChanged: {
        if (model) labelText = model.data(model.index(modelIndex, 0), 257)
    }
}
