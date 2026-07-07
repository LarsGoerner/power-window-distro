import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 4
        spacing: 2
        RowLayout {
            Button {
                text: "Scan"
                enabled: !WifiManager.scanning && !WifiManager.connecting
                font.pixelSize: 6
                implicitHeight: 12
                onClicked: WifiManager.startScan()
            }
            Label {
                text: WifiManager.statusMessage
                color: Theme.fgColor
                font.pixelSize: 6
                Layout.fillWidth: true
            }
        }
        ComboBox {
            id: ssidCombo
            model: WifiManager.scanResults
            font.pixelSize: 6
            implicitHeight: 12
            Layout.fillWidth: true
        }
        TextField {
            id: passwordField
            placeholderText: "Password"
            echoMode: TextInput.Password
            font.pixelSize: 6
            implicitHeight: 12
            Layout.fillWidth: true
            topPadding: 2
            bottomPadding: 2
        }
        Button {
            text: "Connect"
            enabled: ssidCombo.currentIndex >= 0 && passwordField.text.length > 0
            font.pixelSize: 6
            implicitHeight: 12
            Layout.alignment: Qt.AlignHCenter
            onClicked: WifiManager.connectToNetwork(ssidCombo.currentText, passwordField.text)
        }
    }
}