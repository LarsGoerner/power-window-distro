import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        spacing: Theme.spacingL
        anchors.margins: Theme.spacingL
        // Brightness
        Label {
            text: "Brightness"
            font.pixelSize: Theme.fontSizeM
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
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingS
            Label {
                text: "Theme"
                font.pixelSize: Theme.fontSizeM
                color: Theme.fgColor
            }
            ComboBox {
                id: themeCombo
                Layout.fillWidth: true
                implicitHeight: 42
                model: Theme.themeNames
                font.pixelSize: Theme.fontSizeM
                currentIndex: {
                    var names = Theme.themeNames
                    var cur = Theme.currentTheme
                    for (var i = 0; i < names.length; i++) {
                        if (names[i] === cur) return i
                    }
                    return 0
                }
                onActivated: Theme.setThemeByIndex(index)
            }
        }
        // Screensaver
        Label {
            text: "Screensaver"
            font.pixelSize: Theme.fontSizeM;
            color: Theme.fgColor
        }
        RowLayout {
            spacing: Theme.spacingS
            Layout.fillWidth: true
            ButtonGroup { id: ssGroup }
            RadioButton {
                text: "Aus"
                id: offRadio
                Layout.fillWidth: true
                implicitHeight: 30
                spacing: Theme.spacingS
                font.pixelSize: Theme.fontSizeM
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
                spacing: Theme.spacingS
                font.pixelSize: Theme.fontSizeM
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
                spacing: Theme.spacingS
                font.pixelSize: Theme.fontSizeM
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