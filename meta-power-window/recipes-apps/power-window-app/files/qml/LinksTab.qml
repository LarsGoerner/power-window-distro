import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Item {
    ColumnLayout {
        width: parent.width
        spacing: Theme.spacingL
        anchors.margins: Theme.spacingL
        RowLayout {
            Layout.fillWidth: true
            spacing: Theme.spacingS
            Label {
                text: "News Feed URL"
                font.pixelSize: Theme.fontSizeM
                color: Theme.fgColor
            }
            ComboBox {
                id: feedCombo
                Layout.fillWidth: true
                model: NewsTicker.feedNames
                font.pixelSize: Theme.fontSizeM
                currentIndex: {
                    var names = NewsTicker.feedNames
                    var cur = NewsTicker.currentFeed
                    for (var i = 0; i < names.length; i++) {
                        if (names[i] === cur) { return i }
                    }
                    return 0
                }
                onActivated: NewsTicker.currentFeed = currentText
            }
        }
    }
}