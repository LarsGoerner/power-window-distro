import QtQuick 2.15
import QtQuick.Controls 2.15
import QtCharts 2.15
import PowerWindow 1.0

Page {
    background: Rectangle { color: Theme.bgColor }
    
    ChartView {
        id: chart
        anchors.fill: parent
        anchors.margins: 2
        backgroundColor: "transparent"
        legend.visible: false
        legend.labelColor: Theme.fgColor
        legend.font.pixelSize: 6
        legend.alignment: Qt.AlignBottom
        antialiasing: false
        margins.top: 0
        margins.left: 0
        margins.right: 0
        margins.bottom: 0
        BarCategoryAxis {
            id: dateAxis
            gridVisible: false
            labelsFont.pixelSize: 6
            labelsColor: Theme.fgColor
        }
        ValueAxis {
            id: hiddenX
            min: -0.2
            max: 0
            visible: false
        }
        ValueAxis {
            id: tempAxis
            min: 0
            max: 30
            titleText: "Temperatur [°C]"
            titleFont.pixelSize: 6
            titleFont.bold: false
            titleBrush: Theme.fgColor
            labelFormat: "%.0f"
            labelsFont.pixelSize: 6
            labelsColor: Theme.fgColor
            gridLineColor: Theme.accentColor
        }
        ValueAxis {
            id: rainAxis
            min: 0
            max: 10
            titleText: "Niederschlag [mm]"
            titleFont.pixelSize: 6
            titleFont.bold: false
            titleBrush: Theme.fgColor
            labelFormat: "%.0f"
            labelsFont.pixelSize: 6
            labelsColor: Theme.fgColor
            gridVisible: false
        }
        BarSeries {
            id: rainBars
            barWidth: 0.5
            BarSet {
                id: rainSet
                label: "Niederschlag"
                color: Theme.accentColor
            }
            axisX: dateAxis
            axisYRight: rainAxis
        }
        LineSeries {
            id: maxTLine
            name: "Max"
            color: Theme.fgColor
            width: 2
            pointsVisible: true
            pointLabelsVisible: false
            axisX: hiddenX
            axisY: tempAxis
        }
        LineSeries {
            id: minTLine
            name: "Min"
            color: Theme.fgColor
            width: 2
            pointsVisible: true
            pointLabelsVisible: false
            axisX: hiddenX
            axisY: tempAxis
            style: Qt.DashLine
        }
    }

    function rebuild() {
        var p = WeatherFetcher.forecast
        if (!p || !p.length) return
        var n = p.length

        var lo = 999, hi = -999, mr = 0
        for (var i = 0; i < n; i++) {
            lo = Math.min(lo, p[i].tempMin)
            hi = Math.max(hi, p[i].tempMax)
            mr = Math.max(mr, p[i].rain)
        }
        var range = hi - lo || 1
        var pad = range * 0.1

        hiddenX.max = n - 0.3
        tempAxis.min = lo - pad
        tempAxis.max = hi + pad
        rainAxis.max = mr || 1

        var cats = []
        for (var i = 0; i < n; i++) { cats.push(p[i].day) }
        dateAxis.categories = cats

        maxTLine.clear()
        minTLine.clear()
        for (var i = 0; i < n; i++) {
            maxTLine.append(i + 0.5, p[i].tempMax)
            minTLine.append(i + 0.5, p[i].tempMin)
        }

        var vals = []
        for (var i = 0; i < n; i++) { vals.push(p[i].rain) }
        rainSet.values = vals
    }
    Component.onCompleted: rebuild()
    Connections {
        target: WeatherFetcher
        onForecastChanged: rebuild()
    }
}
