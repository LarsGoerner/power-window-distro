import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.VirtualKeyboard 2.15
import PowerWindow 1.0

ApplicationWindow {
    id: window
    visible: true
    title: "Power Window app"

    background: Rectangle { color: Theme.bgColor }

    SwipeView {
        id: view
        anchors.fill: parent
        currentIndex: indicator.currentIndex
        AmbientPage {}
        WeatherPage {}
        SettingsPage{}
    }

    PageIndicator {
        id: indicator
        count: view.count
        currentIndex: view.currentIndex
        anchors.bottom: view.bottom
        anchors.horizontalCenter: view.horizontalCenter
        anchors.bottomMargin: 45
    }

    InputPanel {
        id: inputPanel
        z: 99
        y: window.height
        width: window.width
        states: State {
                name: "visible"
                when: inputPanel.active
                PropertyChanges { target: inputPanel; y: window.height - inputPanel.height }
        }
        transitions: Transition {
                from: ""
                to: "visible"
                reversible: true
                NumberAnimation {
                        properties: "y"
                        duration: 200
                        easing.type: Easing.InOutQuad
                }
        }
    }
}