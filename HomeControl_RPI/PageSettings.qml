import QtQuick 2.9
import QtQuick.Controls 2.4 as C2
import QtQuick.Controls 1.4 as C1
import QtQuick.Layouts 1.3

import "style"
import "components"

HCPage {
    id: root

    title: qsTr("Settings")

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: listView
            Layout.fillHeight: true
            Layout.fillWidth: true

            model: getModel()

            delegate: Item {
                height: 30
                width: listView.width

                HCSimpleLabel {
                    labelText: modelData
                    labelValue: DataBridge.appConfigModel[modelData]
                    labelWidth: 300

                    isValid: true
                    allowInput: true
                    inputHandler: function(newValue) {
                        DataBridge.setAppConfig(modelData, newValue)
                    }
                }
            }

            function getModel() {
                var returnModel = [];

                for (var prop in DataBridge.appConfigModel) {
                    returnModel.push(prop);
                }

                return returnModel;
            }

            function readValues(anObject) {
                for (var prop in anObject) {
                    console.log("Object item:", prop, "=", anObject[prop])
                }
            }
        }

        Item {
            Layout.preferredHeight: 100
            Layout.fillWidth: true
        }
    }

}
