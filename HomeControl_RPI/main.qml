import QtQuick 2.9
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.VirtualKeyboard 2.2

ApplicationWindow {
    id: window
    visible: true
    width: 1024
    height: 600
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
    }

    SwipeView {
        id: swipeView
        anchors.fill: parent
        currentIndex: tabBar.currentIndex

        Page1 {
            id: page1
        }

        Page2 {
            id: page2
        }

        Page3 {
            id: page3
        }

        Page4 {
            id: page4
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
