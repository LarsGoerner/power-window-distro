import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Page {
    background: Rectangle { color: Theme.bgColor }
    ColumnLayout {
        anchors.fill: parent
        TabBar {
            id: settingsTabs
            Layout.fillWidth: true
            implicitHeight: 20
            topPadding: 0
            bottomPadding: 0
            TabButton { text: "Display"; font.pixelSize: 8; topPadding: 2; bottomPadding: 2; implicitHeight: 20 }
            TabButton { text: "WiFi"; font.pixelSize: 8; topPadding: 2; bottomPadding: 2; implicitHeight: 20 }
            TabButton { text: "Growatt"; font.pixelSize: 8; topPadding: 2; bottomPadding: 2; implicitHeight: 20 }
            TabButton { text: "Update"; font.pixelSize: 8; topPadding: 2; bottomPadding: 2; implicitHeight: 20 }
        }
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: settingsTabs.currentIndex
            DisplayTab {}
            WifiTab {}
            GrowattTab {}
            UpdateTab {}
        }
    }
}