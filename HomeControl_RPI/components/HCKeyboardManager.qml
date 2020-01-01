pragma Singleton
import QtQuick 2.0

import "../style"

Item {
    id: root

    property var keyboardRef
    property var currentCB

    function invoke(currentValue, cb, imh, validator, font) {
        currentCB = cb

        keyboardRef.currentInput.text = currentValue
        keyboardRef.currentInput.inputMethodHints = imh
        keyboardRef.currentInput.validator = validator
        keyboardRef.currentInput.font.pointSize = Style.fontPointSizeKeyboard
        keyboardRef.open()
    }

    onKeyboardRefChanged: {
        if (keyboardRef) {
            keyboardRef.onInputAccepted.connect(function(newText) {
                keyboardRef.close()
                currentCB(newText)
            })

            keyboardRef.onInputCanceled.connect(function() {
                keyboardRef.close()
            })
        }
    }

}
