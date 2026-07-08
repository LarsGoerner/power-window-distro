import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 0
        spacing: 6
        RowLayout {
            Button {
                text: "Scan"
                enabled: !WifiManager.scanning && !WifiManager.connecting
                font.pixelSize: 18
                implicitHeight: 36
                onClicked: WifiManager.startScan()
            }
            Label {
                text: WifiManager.statusMessage
                color: Theme.fgColor
                font.pixelSize: 18
                Layout.fillWidth: true
            }
        }
        ComboBox {
            id: ssidCombo
            model: WifiManager.scanResults
            font.pixelSize: 18
            implicitHeight: 36
            Layout.fillWidth: true
        }
        TextField {
            id: passwordField
            placeholderText: "Password"
            echoMode: TextInput.Password
            font.pixelSize: 18
            implicitHeight: 36
            Layout.fillWidth: true
            topPadding: 6
            bottomPadding: 6
        }
        Button {
            text: "Connect"
            enabled: ssidCombo.currentIndex >= 0 && passwordField.text.length > 0
            font.pixelSize: 18
            implicitHeight: 36
            Layout.alignment: Qt.AlignHCenter
            onClicked: WifiManager.connectToNetwork(ssidCombo.currentText, passwordField.text)
        }
    }
}