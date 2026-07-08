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
                anchors.fill: parent
                spacing: Theme.spacingM
                Label {
                    text: modelData.title || ""
                    font.pixelSize: Theme.fontSizeXL
                    color: Theme.fgColor
                    wrapMode: Text.WordWrap
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignTop
                }
                Rectangle {
                    color: Theme.accentColor
                    opacity: 0.3
                    height: 1
                    Layout.fillWidth: true
                }
                RowLayout {
                    Layout.fillWidth: true
                    Layout.fillHeight: true
                    Label {
                        text: modelData.description || ""
                        font.pixelSize: Theme.fontSizeL
                        color: Theme.fgColor
                        opacity: 0.8
                        wrapMode: Text.WordWrap
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                    }
                    Image {
                        source: modelData.imageUrl || ""
                        fillMode: Image.PreserveAspectFit
                        asynchronous: true
                        visible: status !== Image.Error
                              && status !== Image.Null
                              && modelData.imageUrl
                        Layout.fillWidth: true
                        Layout.fillHeight: true
                        Layout.alignment: Qt.AlignVCenter
                    }
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