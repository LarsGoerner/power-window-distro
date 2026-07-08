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
            implicitHeight: 60
            topPadding: 0
            bottomPadding: 0
            TabButton { text: "Display"; font.pixelSize: Theme.fontSizeL; topPadding: 6; bottomPadding: 6; implicitHeight: 60 }
            TabButton { text: "WiFi";    font.pixelSize: Theme.fontSizeL; topPadding: 6; bottomPadding: 6; implicitHeight: 60 }
            TabButton { text: "Growatt"; font.pixelSize: Theme.fontSizeL; topPadding: 6; bottomPadding: 6; implicitHeight: 60 }
            TabButton { text: "Links";   font.pixelSize: Theme.fontSizeL; topPadding: 6; bottomPadding: 6; implicitHeight: 60 }
            TabButton { text: "Update";  font.pixelSize: Theme.fontSizeL; topPadding: 6; bottomPadding: 6; implicitHeight: 60 }
        }
        StackLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: settingsTabs.currentIndex
            DisplayTab {}
            WifiTab {}
            GrowattTab {}
            LinksTab {}
            UpdateTab {}
        }
    }
}