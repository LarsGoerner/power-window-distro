import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.VirtualKeyboard 2.15
import PowerWindow 1.0

ApplicationWindow {
    id: window
    visible: true
    width: 1280
    height: 720
    title: "Power Window app"

    background: Rectangle { color: Theme.bgColor }

    SwipeView {
        id: view
        anchors.fill: parent
        currentIndex: indicator.currentIndex

        // Ambient
        Page {
            background: Rectangle { color: Theme.bgColor }
            Label {
                text: "Ambient"
                anchors.centerIn: parent
                font.pixelSize: 28
                color: Theme.fgColor
            }
        }

        // Weather
        Page {
            background: Rectangle { color: Theme.bgColor }
            Label {
                text: "Weather"
                anchors.centerIn: parent
                font.pixelSize: 28
                color: Theme.fgColor
            }
        }

        // Settings
        Page {
            background: Rectangle { color: Theme.bgColor }
            ColumnLayout {
                anchors.fill: parent
                TabBar {
                    id: settingsTabs
                    Layout.fillWidth: true
                    implicitHeight: 20
                    topPadding: 0
                    bottomPadding: 0
                    TabButton { text: "Display"; font.pixelSize: 8; topPadding: 2; bottomPadding: 2; implicitHeight: 20 }
                    TabButton { text: "WiFi"; font.pixelSize: 8; topPadding: 2; bottomPadding: 2; implicitHeight: 20 }
                    TabButton { text: "Growatt"; font.pixelSize: 8; topPadding: 2; bottomPadding: 2; implicitHeight: 20 }
                    TabButton { text: "Update"; font.pixelSize: 8; topPadding: 2; bottomPadding: 2; implicitHeight: 20 }
                }
                StackLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    currentIndex: settingsTabs.currentIndex

                    // Display Settings
                    Item {
                        ColumnLayout {
                            anchors.fill: parent
                            anchors.margins: 4
                            spacing: 4

                            // Brightness
                            Label {
                                text: "Brightness"
                                font.pixelSize: 6
                                color: Theme.fgColor
                            }

                            RowLayout {
                                Slider {
                                    id: brightnessSlider
                                    Layout.fillWidth: true
                                    height: 16
                                    from: 20
                                    to: 100
                                    value: Backlight.brightness
                                    onMoved: Backlight.setBrightness(value)

                                    handle: Rectangle {
                                        width: height
                                        height: slider.height * 1.2
                                        radius: height / 2
                                        color: Theme.accentColor
                                    }
                                }
                            }

                            // Theme
                            Label {
                                text: "Theme"
                                font.pixelSize: 6
                                color: Theme.fgColor
                            }

                            Flow {
                                Layout.fillWidth: true
                                height: 20
                                spacing: 2
                                Repeater {
                                    model: Theme.themeDisplayNames
                                    delegate: Button {
                                        text: modelData
                                        font.pixelSize: 6
                                        implicitHeight: 20
                                        width: 50
                                        highlighted: Theme.currentTheme === Theme.themes[index]
                                        onClicked: Theme.setThemeByIndex(index)
                                    }
                                }
                            }
                        }
                    }

                    // WiFi
                    Item {
                        ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 4
                                spacing: 2

                                RowLayout {
                                        Button {
                                                text: "Scan"
                                                enabled: !WifiManager.scanning && !WifiManager.connecting
                                                font.pixelSize: 6
                                                implicitHeight: 12
                                                onClicked: WifiManager.startScan()
                                        }
                                        Label {
                                                text: WifiManager.statusMessage
                                                color: Theme.fgColor
                                                font.pixelSize: 6
                                                Layout.fillWidth: true
                                        }
                                }

                                ComboBox {
                                        id: ssidCombo
                                        model: WifiManager.scanResults
                                        font.pixelSize: 6
                                        implicitHeight: 12
                                        Layout.fillWidth: true
                                }

                                TextField {
                                        id: passwordField
                                        placeholderText: "Password"
                                        echoMode: TextInput.Password
                                        font.pixelSize: 6
                                        implicitHeight: 12
                                        Layout.fillWidth: true
                                        topPadding: 2
                                        bottomPadding: 2
                                }

                                Button {
                                        text: "Connect"
                                        enabled: ssidCombo.currentIndex >= 0 && passwordField.text.length > 0
                                        font.pixelSize: 6
                                        implicitHeight: 12
                                        Layout.alignment: Qt.AlignHCenter
                                        onClicked: WifiManager.connectToNetwork(ssidCombo.currentText, passwordField.text)
                                }
                        }
                    }

                    // Growatt Login
                    Item {
                        
                    }

                    // Update
                    Item {
                        ColumnLayout {
                                anchors.fill: parent
                                anchors.margins: 2
                                spacing: 2

                                Button {
                                        text: "Update"
                                        font.pixelSize: 8
                                        implicitHeight: 16
                                        Layout.alignment: Qt.AlignHCenter
                                        enabled: !Updater.updateRunning
                                        onClicked: Updater.startUpdate();
                                }
                                ScrollView {
                                        Layout.fillWidth: true
                                        Layout.fillHeight: true

                                        TextArea {
                                                text: Updater.updateLog
                                                readOnly: true
                                                font.pixelSize: 6
                                                font.family: "monospace"
                                                color: Theme.fgColor
                                                background: Rectangle {
                                                        color: Theme.bgColor
                                                        border.color: Theme.accentColor
                                                        border.width: 1
                                                }
                                        }
                                }
                        }
                    }
                }
            }
        }
    }

    PageIndicator {
        id: indicator
        count: view.count
        currentIndex: view.currentIndex
        anchors.bottom: view.bottom
        anchors.horizontalCenter: view.horizontalCenter
        anchors.bottomMargin: 15
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