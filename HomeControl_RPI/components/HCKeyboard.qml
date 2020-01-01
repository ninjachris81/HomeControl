import QtQuick 2.0
import QtQuick.VirtualKeyboard 2.2
import QtQuick.Controls 2.4

Popup {
    id: root

    modal: true

    property alias currentInput: currentInput

    signal inputAccepted(string newText)
    signal inputCanceled()

    onOpened: {
        currentInput.forceActiveFocus()
    }

    TextInput {
        id: currentInput

        onAccepted: {
            inputAccepted(currentInput.text)
        }
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: root.height
        width: root.width

        onActiveChanged: {
            if (!active) {
                inputCanceled()
            }
        }

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: root.height - inputPanel.height
            }
        }
        transitions: Transition {
            from: ""
            to: "visible"
            reversible: true
            ParallelAnimation {
                NumberAnimation {
                    properties: "y"
                    duration: 250
                    easing.type: Easing.InOutQuad
                }
            }
        }
    }

}
