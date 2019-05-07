import QtQuick 2.9
import QtQuick.Window 2.11
import QtQuick.Controls 2.2
import QtQuick.VirtualKeyboard 2.2

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 600
    title: qsTr("Hello World")

    visibility: Window.FullScreen

    color: "red"

    header: TabBar {
        id: tabBar
        currentIndex: 0

        TabButton {
            text: "page1.title"
        }
        TabButton {
            text: "page2.title"
        }
        TabButton {
            text: "page3.title"
        }
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Item {
            id: page1
        }

        Item {
            id: page2
        }

        Item {
            id: page3
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
}
