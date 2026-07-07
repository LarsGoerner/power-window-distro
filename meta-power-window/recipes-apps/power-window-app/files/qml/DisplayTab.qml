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
                height: 16
                from: 20
                to: 100
                value: Backlight.brightness
                onMoved: Backlight.setBrightness(value)
                handle: Rectangle {
                    width: height
                    height: brightnessSlider.height * 1.2
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