import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Shapes 1.15
import PowerWindow 1.0

Page {
    background: Rectangle { color: Theme.bgColor }

    property string currentTime: ""
    Timer {
        interval: (60 - new Date().getSeconds()) * 1000
        running: true
        repeat: true
        onTriggered: { currentTime = Qt.formatTime(new Date(), "hh:mm") }
    }

    component BigLabel: Label {
        anchors.fill: parent
        anchors.margins: Theme.spacingM
        horizontalAlignment: Label.AlignHCenter
        verticalAlignment: Label.AlignVCenter
        font.pixelSize: 100
        fontSizeMode: Label.Fit
        color: Theme.fgColor
    }

    component ItemFrame: Rectangle {
        Layout.fillWidth: true
        Layout.fillHeight: true
        color: "transparent"
        border.color: Theme.accentColor
        border.width: 2
        radius: 4
    }

    RowLayout {
        id: mainLayout
        anchors.fill: parent
        spacing: Theme.spacingM

        // Outdoor data
        ColumnLayout {
            Layout.preferredWidth: mainLayout.width * 0.35
            Layout.fillHeight: true
            spacing: Theme.spacingM

            ItemFrame {
                Layout.preferredHeight: 2
                BigLabel { text: qsTr("Draußen") }
            }

            // Temperature
            ItemFrame {
                Layout.preferredHeight: 5
                BigLabel {
                    text: WeatherFetcher.ready
                          ? qsTr(Math.round(WeatherFetcher.temperature) + "°C")
                          : qsTr("--°C")
                }
            }

            // Humidity
            ItemFrame {
                Layout.preferredHeight: 5
                BigLabel {
                    text: WeatherFetcher.ready
                          ? qsTr(Math.round(WeatherFetcher.humidity) + "%")
                          : qsTr("--%")
                }
            }
        }

        // Wind / Time
        ColumnLayout {
            Layout.preferredWidth: mainLayout.width * 0.3
            Layout.fillHeight: true
            spacing: Theme.spacingM

            // Weather Icon
            ItemFrame {
                Layout.preferredHeight: 2

                Image {
                    anchors.centerIn: parent
                    source: WeatherFetcher.ready
                        ? "file:///usr/share/power-window-app/weatherIcons/" + WeatherFetcher.iconCode + ".png"
                        : ""
                    sourceSize.width: 100
                    sourceSize.height: 100
                    width: 128
                    height: 128
                }
            }

            // Wind Space
            ItemFrame { Layout.preferredHeight: 8 }

            // Time
            ItemFrame {
                Layout.preferredHeight: 2
                BigLabel { text: qsTr(currentTime) }
            }
        }

        // Indoor data
        ColumnLayout {
            Layout.preferredWidth: mainLayout.width * 0.35
            Layout.fillHeight: true
            spacing: Theme.spacingM

            ItemFrame {
                Layout.preferredHeight: 2
                BigLabel { text: qsTr("Zimmer") }
            }

            // Temperature
            ItemFrame {
                Layout.preferredHeight: 5
                BigLabel {
                    text: DhtSensor.temperature > 0
                          ? qsTr(Math.round(DhtSensor.temperature) + "°C")
                          : qsTr("--°C")
                }
            }

            // Humidity
            ItemFrame {
                Layout.preferredHeight: 5
                BigLabel {
                    text: DhtSensor.humidity > 0
                          ? qsTr(Math.round(DhtSensor.humidity) + "%")
                          : qsTr("--%")
                }
            }
        }
    }

    Item {
        width: 500
        height: 500
        anchors.centerIn: parent

        Windrose {
            anchors.fill: parent
            windDirection: WeatherFetcher.windDirection
            backgroundColor: Theme.bgColor
            normColor: Theme.fgColor
            highlightColor: Theme.accentColor
        }

        ColumnLayout {
            anchors.centerIn: parent
            spacing: Theme.spacingM

            Label {
                horizontalAlignment: Label.AlignHCenter
                color: Theme.fgColor
                font.pixelSize: Theme.fontSizeXL
                text: qsTr(WeatherFetcher.windSpeed + " m/s")
            }
            Label {
                horizontalAlignment: Label.AlignHCenter
                color: Theme.accentColor
                opacity: 0.8
                font.pixelSize: Theme.fontSizeL
                text: qsTr(WeatherFetcher.windGust + " m/s")
            }
        }
    }
}
