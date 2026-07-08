import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 12
        spacing: 12
        // Brightness
        Label {
            text: "Brightness"
            font.pixelSize: 18
            color: Theme.fgColor
        }
        RowLayout {
            Slider {
                id: brightnessSlider
                Layout.fillWidth: true
                implicitHeight: 30
                topPadding: 0
                bottomPadding: 0
                from: 20
                to: 100
                value: Backlight.brightness
                onMoved: Backlight.setBrightness(value)
            }
        }
        // Theme
        Label {
            text: "Theme"
            font.pixelSize: 18
            color: Theme.fgColor
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 6
            Repeater {
                model: Theme.themeDisplayNames
                delegate: Button {
                    text: modelData
                    font.pixelSize: 18
                    implicitHeight: 42
                    Layout.fillWidth: true
                    highlighted: Theme.currentTheme === Theme.themes[index]
                    onClicked: Theme.setThemeByIndex(index)
                }
            }
        }
        // Screensaver
        Label {
            text: "Screensaver"
            font.pixelSize: 18;
            color: Theme.fgColor
        }
        RowLayout {
            spacing: 6
            Layout.fillWidth: true
            ButtonGroup { id: ssGroup }
            RadioButton {
                text: "Aus"
                id: offRadio
                Layout.fillWidth: true
                implicitHeight: 30
                spacing: 6
                font.pixelSize: 18
                ButtonGroup.group: ssGroup
                checked: ScreenSaver.mode === "off"
                onClicked: ScreenSaver.mode = "off"
                contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: Theme.fgColor
                        leftPadding: parent.indicator.implicitWidth + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                }
            }
            RadioButton {
                text: "Dimmed Mode"
                id: dimRadio
                Layout.fillWidth: true
                implicitHeight: 30
                spacing: 6
                font.pixelSize: 18
                ButtonGroup.group: ssGroup
                checked: ScreenSaver.mode === "dim"
                onClicked: ScreenSaver.mode = "dim"
                contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: Theme.fgColor
                        leftPadding: parent.indicator.implicitWidth + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                }
            }
            RadioButton {
                text: "Max Mode"
                id: maxRadio
                Layout.fillWidth: true
                implicitHeight: 30
                spacing: 6
                font.pixelSize: 18
                ButtonGroup.group: ssGroup
                checked: ScreenSaver.mode === "full"
                onClicked: ScreenSaver.mode = "full"
                contentItem: Text {
                        text: parent.text
                        font: parent.font
                        color: Theme.fgColor
                        leftPadding: parent.indicator.implicitWidth + parent.spacing
                        verticalAlignment: Text.AlignVCenter
                        elide: Text.ElideRight
                }
            }
        }
    }
}