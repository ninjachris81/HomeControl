pragma Singleton
import QtQuick 2.0

import "../style"

Item {
    property Item toastComponent
    property var messages: []
    property bool showingMessage: false
    property int originalHeight

    Timer {
        id: toastTimer

        interval: 5000
        repeat: false
        onTriggered: {
            checkNextToast();
        }
    }

    onToastComponentChanged: {
        if (!toastComponent) return;

        toastComponent.visible = false;
        originalHeight = toastComponent.height
        toastComponent.height = 0
    }

    function showToast(msg) {
        toastComponent.visible = true;
        messages.push(msg);
        checkNextToast();
    }

    function checkNextToast() {
        if (messages.length==0) {
            toastComponent.height = 0;
            return;
        } else {
            if (showingMessage) return;

            var msg = messages.pop();
            toastComponent.text = msg;
            toastComponent.height = originalHeight;
            toastTimer.start();
        }
    }


}
