import QtQuick 2.0
import QtQuick.Controls 2.4

Item {
    property var model
    property var modelIndex

    property bool allowInput: true
    property var inputHandler

    Connections {
        target: model

        onDataChanged: {
            mySwitch.checked = model ? model.data(model.index(modelIndex, 0), 258) : false
        }
    }

    onModelChanged: {
        mySwitch.checked = model ? model.data(model.index(modelIndex, 0), 258) : false
    }

    Switch {
        id: mySwitch
        enabled: allowInput
        checked: model.data(model.index(modelIndex, 0), 258)

        onToggled: {
            if (typeof(inputHandler)=="function") inputHandler(checked);
        }
    }
}
