import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Page {
    background: Rectangle { color: Theme.bgColor }

    BusyIndicator {
        anchors.centerIn: parent
        running: !NewsTicker.ready
    }

    ListView {
        id: list
        clip: true
        anchors.fill: parent
        anchors.margins: Theme.spacingL
        orientation: ListView.Vertical
        snapMode: ListView.SnapOneItem
        highlightRangeMode: ListView.StrictlyEnforceRange
        boundsBehavior: Flickable.StopAtBounds
        model: NewsTicker.headlines
        delegate: Item {
            width: list.width
            height: list.height
            ColumnLayout {
                anchors.verticalCenter: parent.verticalCenter
                anchors.left: parent.left
                anchors.right: parent.right
                spacing: Theme.spacingM
                Label {
                    text: modelData.title || ""
                    font.pixelSize: Theme.fontSizeXL
                    color: Theme.fgColor
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                }
                Label {
                    text: modelData.description || ""
                    font.pixelSize: Theme.fontSizeL
                    color: Theme.fgColor
                    opacity: 0.8
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    maximumLineCount: 8
                    elide: Text.ElideRight
                }
            }
        }
        Label {
            anchors.centerIn: parent
            text: "Keine Nachrichten"
            font.pixelSize: Theme.fontSizeL
            color: Theme.fgColor
            visible: NewsTicker.ready && list.count === 0
        }
    }
    Label {
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: Theme.spacingS
        text: (list.currentIndex + 1) + " / " + list.count
        font.pixelSize: Theme.fontSizeM
        color: Theme.fgColor
        opacity: 0.6
        visible: NewsTicker.ready && list.count > 0
    }
}