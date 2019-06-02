import QtQuick 2.9
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.VirtualKeyboard 2.2

import "style"
import "components"

ApplicationWindow {
    id: window
    visible: true
    width: Style.screenWidth
    height: Style.screenHeight
    title: qsTr("Tabs")

    visibility: Qt.platform.os==="linux" ? Window.FullScreen : Window.Windowed

    header: TabBar {
        id: tabBar
        currentIndex: swipeView.currentIndex

        TabButton {
            text: page1.title
            font.pointSize: Style.fontPointSize
        }
        TabButton {
            text: page2.title
            font.pointSize: Style.fontPointSize
        }
        TabButton {
            text: page3.title
            font.pointSize: Style.fontPointSize
        }
        TabButton {
            text: page4.title
            font.pointSize: Style.fontPointSize
        }
        TabButton {
            text: page5.title
            font.pointSize: Style.fontPointSize
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        PageHeating {
            id: page1
        }

        PageWaterManagement {
            id: page2
        }

        PageSolar {
            id: page3
        }

        PageLogs {
            id: page4
        }

        PageSettings {
            id: page5
        }
    }

    InputPanel {
        id: inputPanel
        z: 99
        x: 0
        y: window.height
        width: window.width

        states: State {
            name: "visible"
            when: inputPanel.active
            PropertyChanges {
                target: inputPanel
                y: window.height - inputPanel.height
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


    Rectangle {
        id: toastItem

        color: "grey"

        property alias text: popupLabel.text

        height: 40
        width: Style.screenWidth / 2

        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        radius: 10

        HCLabel {
            id: popupLabel

            anchors.fill: parent

            visible: parent.height>0

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Behavior on height {
            NumberAnimation { duration: 300 }
        }
    }

    Component.onCompleted: {
        HCToastManager.toastComponent = toastItem
    }

    Connections {
        target: DataBridge

        onIsConnectedChanged: {
            if (DataBridge.isConnected) {
                HCToastManager.showToast(qsTr("Connected"));
            }
        }
    }
}
