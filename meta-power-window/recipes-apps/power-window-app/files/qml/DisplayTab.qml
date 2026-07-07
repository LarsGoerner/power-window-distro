import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

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
                implicitHeight: 10
                topPadding: 0
                bottomPadding: 0
                from: 20
                to: 100
                value: Backlight.brightness
                onMoved: Backlight.setBrightness(value)
                handle: Rectangle {
                    x: brightnessSlider.leftPadding + brightnessSlider.visualPosition * (brightnessSlider.availableWidth - width)
                    y: (parent.height - height) / 2
                    width: 8
                    height: 8
                    radius: 4
                    color: Theme.fgColor
                }
                background: Rectangle {
                    y: (parent.height - height) / 2
                    width: parent.availableWidth
                    height: 2
                    color: Theme.fgColor
                    opacity: 0.3
                }
            }
        }
        // Theme
        Label {
            text: "Theme"
            font.pixelSize: 6
            color: Theme.fgColor
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: 2
            Repeater {
                model: Theme.themeDisplayNames
                delegate: Button {
                    text: modelData
                    font.pixelSize: 6
                    implicitHeight: 14
                    Layout.fillWidth: true
                    width: 50
                    highlighted: Theme.currentTheme === Theme.themes[index]
                    onClicked: Theme.setThemeByIndex(index)
                }
            }
        }
        // Screensaver
        Label {
            text: "Screensaver"
            font.pixelSize: 6;
            color: Theme.fgColor
        }
        RowLayout {
            spacing: 2
            Layout.fillWidth: true
            ButtonGroup { id: ssGroup }
            RadioButton {
                text: "Aus"
                id: offRadio
                Layout.fillWidth: true
                implicitHeight: 10
                spacing: 2
                topPadding: 0
                bottomPadding: 0
                leftPadding: 0
                rightPadding: 0
                font.pixelSize: 6
                ButtonGroup.group: ssGroup
                checked: ScreenSaver.mode === "off"
                onClicked: ScreenSaver.mode = "off"
                indicator: Rectangle {
                    implicitWidth: 8
                    implicitHeight: 8
                    width: 8
                    height: 8
                    radius: 4
                    border.width: 1
                    border.color: Theme.fgColor
                    color: offRadio.checked ? Theme.fgColor : "transparent"
                }
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
                implicitHeight: 10
                spacing: 2
                topPadding: 0
                bottomPadding: 0
                leftPadding: 0
                rightPadding: 0
                font.pixelSize: 6
                ButtonGroup.group: ssGroup
                checked: ScreenSaver.mode === "dim"
                onClicked: ScreenSaver.mode = "dim"
                indicator: Rectangle {
                    implicitWidth: 8
                    implicitHeight: 8
                    width: 8
                    height: 8
                    radius: 4
                    border.width: 1
                    border.color: Theme.fgColor
                    color: dimRadio.checked ? Theme.fgColor : "transparent"
                }
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
                implicitHeight: 10
                spacing: 2
                topPadding: 0
                bottomPadding: 0
                leftPadding: 0
                rightPadding: 0
                font.pixelSize: 6
                ButtonGroup.group: ssGroup
                checked: ScreenSaver.mode === "full"
                onClicked: ScreenSaver.mode = "full"
                indicator: Rectangle {
                    implicitWidth: 8
                    implicitHeight: 8
                    width: 8
                    height: 8
                    radius: 4
                    border.width: 1
                    border.color: Theme.fgColor
                    color: maxRadio.checked ? Theme.fgColor : "transparent"
                }
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