import QtQuick 2.12

Item {
    id: root

    property bool value
    property bool isValid

    property var model
    property int modelIndex

    Connections {
        target: model

        onDataChanged: {
            value = model ? model.data(model.index(modelIndex, 0), 258) : false
            isValid = model ? model.data(model.index(modelIndex, 0), 259) : false
        }
    }
}
