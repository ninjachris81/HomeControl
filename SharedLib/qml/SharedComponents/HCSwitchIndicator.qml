import QtQuick 2.12
import QtQuick.Particles 2.0

import StyleDef 1.0

Item {
    id: root

    property bool isOn: false
    property string style: ""

    Rectangle {
        radius: 360
        color: "transparent"
        border.color: "black"
        border.width: 2

        anchors.fill: parent

        visible: style==="pump"

        Image {
            id: propeller
            anchors.fill: parent
            anchors.margins: 5

            source: "qrc:/images/icons/motor_prop.png"

            RotationAnimator {
                target: propeller;
                from: 0;
                to: 360;
                duration: 1000
                running: root.isOn
                loops: Animator.Infinite
            }
        }
    }

    Image {
        id: flame
        anchors.fill: parent
        anchors.margins: 5

        visible: style==="flame"

        source: root.isOn ? "qrc:/images/icons/flame_on.png" : "qrc:/images/icons/flame_off.png"

        ParticleSystem {
            anchors.centerIn: flame

            ImageParticle {
                groups: ["flame"]
                source: "qrc:/images/particles/glowdot.png"
                color: "#55ff400f"
                colorVariation: 0.1
            }

            Emitter {
                anchors.centerIn: parent

                group: "flame"

                enabled: root.isOn

                emitRate: 100
                lifeSpan: 500
                size: 25
                endSize: 10
                sizeVariation: 12
                acceleration: PointDirection { y: -40 }
                velocity: AngleDirection { angle: 270; magnitude: 20; angleVariation: 30; magnitudeVariation: 5 }
            }
        }
    }
}
