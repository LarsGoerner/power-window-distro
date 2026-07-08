import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Page {
    background: Rectangle { color: Theme.bgColor }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 24
        spacing: Theme.spacingL

        // --- Indoor ---
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Theme.spacingM

            Label {
                text: "Raum"
                font.pixelSize: Theme.fontSizeXL
                color: Theme.fgColor
            }
            RowLayout {
                spacing: 24
                Label {
                    text: DhtSensor.temperature > 0
                          ? Math.round(DhtSensor.temperature) + "\u00B0C"
                          : "--\u00B0C"
                    font.pixelSize: Theme.fontSizeL
                    color: Theme.fgColor
                }
                Label {
                    text: DhtSensor.humidity > 0
                          ? Math.round(DhtSensor.humidity) + "%"
                          : "--%"
                    font.pixelSize: Theme.fontSizeL
                    color: Theme.fgColor
                }
            }
            Item { Layout.fillHeight: true }
        }

        // --- Vertical separator ---
        Rectangle {
            Layout.fillHeight: true
            width: 3
            color: Theme.fgColor
            opacity: 0.2
        }

        // --- Outdoor ---
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: Theme.spacingS

            Label {
                text: "Draußen"
                font.pixelSize: Theme.fontSizeXL
                color: Theme.fgColor
            }
            RowLayout {
                spacing: Theme.spacingS
                Image {
                    source: WeatherFetcher.ready
                            ? "file:///usr/share/power-window-app/weatherIcons/" + WeatherFetcher.iconCode + ".png"
                            : ""
                    sourceSize.width: 144
                    sourceSize.height: 144
                }
                ColumnLayout {
                    spacing: Theme.spacingS
                    Label {
                        text: WeatherFetcher.ready
                              ? Math.round(WeatherFetcher.temperature) + "°C"
                              : "--°C"
                        font.pixelSize: Theme.fontSizeL
                        color: Theme.fgColor
                    }
                    Label {
                        text: WeatherFetcher.ready
                              ? WeatherFetcher.description
                              : "Lade..."
                        font.pixelSize: Theme.fontSizeL
                        color: Theme.fgColor
                    }
                    Label {
                        text: WeatherFetcher.cityName
                        font.pixelSize: Theme.fontSizeL
                        color: Theme.fgColor
                        opacity: 0.6
                        visible: WeatherFetcher.ready
                    }
                }
            }
            Item { Layout.fillHeight: true }
        }
    }
}
