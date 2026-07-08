import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Item {
    Flickable {
        id: flick
        clip: true
        anchors.fill: parent
        anchors.bottomMargin: Qt.inputMethod.visible
            ? Qt.inputMethod.keyboardRectangle.height / (Screen.devicePixelRatio || 1)
            : 0
        contentHeight: col.height
        boundsBehavior: Flickable.StopAtBounds
        ScrollBar.vertical: ScrollBar { policy: ScrollBar.AsNeeded }
        ColumnLayout {
            id: col
            width: parent.width
            spacing: Theme.spacingL
            anchors.margins: Theme.spacingL
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
                    onActiveFocusChanged: if (activeFocus) { flick.contentY = Math.max(0, y - flick.height * 0.3) }
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
}