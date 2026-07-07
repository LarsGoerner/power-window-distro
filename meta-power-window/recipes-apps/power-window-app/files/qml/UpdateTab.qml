import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import PowerWindow 1.0

Item {
    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 2
        spacing: 2
        Button {
                text: "Update"
                font.pixelSize: 8
                implicitHeight: 16
                Layout.alignment: Qt.AlignHCenter
                enabled: !Updater.updateRunning
                onClicked: Updater.startUpdate();
        }
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            TextArea {
                text: Updater.updateLog
                readOnly: true
                font.pixelSize: 6
                font.family: "monospace"
                color: Theme.fgColor
                background: Rectangle {
                    color: Theme.bgColor
                    border.color: Theme.accentColor
                    border.width: 1
                }
            }
        }
    }
}