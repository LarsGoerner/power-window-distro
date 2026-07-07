import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Page {
    background: Rectangle { color: Theme.bgColor }

    RowLayout {
        anchors.fill: parent
        anchors.margins: 8
        spacing: 8

        // --- Indoor ---
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 6

            Label {
                text: "Raum"
                font.pixelSize: 12
                color: Theme.fgColor
            }
            RowLayout {
                spacing: 8
                Label {
                    text: DhtSensor.temperature > 0
                          ? Math.round(DhtSensor.temperature) + "\u00B0C"
                          : "--\u00B0C"
                    font.pixelSize: 10
                    color: Theme.fgColor
                }
                Label {
                    text: DhtSensor.humidity > 0
                          ? Math.round(DhtSensor.humidity) + "%"
                          : "--%"
                    font.pixelSize: 10
                    color: Theme.fgColor
                }
            }
            Item { Layout.fillHeight: true }
        }

        // --- Vertical separator ---
        Rectangle {
            Layout.fillHeight: true
            width: 1
            color: Theme.fgColor
            opacity: 0.2
        }

        // --- Outdoor ---
        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 6

            Label {
                text: "Drau\u00DFen"
                font.pixelSize: 12
                color: Theme.fgColor
            }
            RowLayout {
                spacing: 8
                Image {
                    source: WeatherFetcher.ready
                            ? "file:///usr/share/power-window-app/weatherIcons/" + WeatherFetcher.iconCode + ".png"
                            : ""
                    sourceSize.width: 48
                    sourceSize.height: 48
                }
                ColumnLayout {
                    spacing: 2
                    Label {
                        text: WeatherFetcher.ready
                              ? Math.round(WeatherFetcher.temperature) + "\u00B0C"
                              : "--\u00B0C"
                        font.pixelSize: 10
                        color: Theme.fgColor
                    }
                    Label {
                        text: WeatherFetcher.ready
                              ? WeatherFetcher.description
                              : "Lade..."
                        font.pixelSize: 10
                        color: Theme.fgColor
                    }
                    Label {
                        text: WeatherFetcher.cityName
                        font.pixelSize: 10
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
