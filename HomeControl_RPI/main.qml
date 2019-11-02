import QtQuick 2.9
import QtQuick.Window 2.11
import QtQuick.Controls 2.4
import QtQuick.VirtualKeyboard 2.2

import hc 1.0

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
        TabButton {
            text: page6.title
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

        PageSecurity {
            id: page4
        }

        PageLogs {
            id: page5
        }

        PageSettings {
            id: page6
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

            color: "white"

            visible: parent.height>0

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignHCenter
        }

        Behavior on height {
            NumberAnimation { duration: 300 }
        }
    }

    footer: Rectangle {
        height: 50

        radius: 10

        color: "grey"

        Rectangle {
            color: parent.color

            height: 10
            anchors.left: parent.left
            anchors.right: parent.right
            anchors.bottom: parent.bottom
        }

        HCLabel {
            id: currentDateTime
            anchors.centerIn: parent

            color: "white"
        }

        HCLabel {
            anchors.right: parent.right
            anchors.rightMargin: 10
            anchors.verticalCenter: parent.verticalCenter

            color: "red"
            text: qsTr("INVALID TIME")
            visible: DataBridge.infos.timeIsOffset

            MouseArea {
                anchors.fill: parent

                onClicked: {
                    HCToastManager.showToast("Server time: " + DataBridge.infos.serverTime)
                }
            }
        }

        Timer {
            id: timer
            interval: 1000
            repeat: true
            running: true

            onTriggered: {
                currentDateTime.text =  Qt.formatDateTime(new Date(), Qt.SystemLocaleDate)
            }
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
