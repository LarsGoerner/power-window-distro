import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: Theme.spacingL
        spacing: Theme.spacingM
        Label {
            text: "News Feed URL"
            font.pixelSize: Theme.fontSizeM
            color: Theme.fgColor
        }
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingS
            TextField {
                id: feedUrlField
                Layout.fillWidth: true
                text: NewsTicker.feedUrl
                font.pixelSize: Theme.fontSizeS
                implicitHeight: 36
                topPadding: 4
                bottomPadding: 4
            }
            Button {
                text: "Aktualisieren"
                font.pixelSize: Theme.fontSizeS
                implicitHeight: 36
                onClicked: NewsTicker.feedUrl = feedUrlField.text
            }
        }
    }
}