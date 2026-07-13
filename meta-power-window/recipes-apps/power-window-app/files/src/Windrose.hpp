#ifndef WINDROSE_HPP_
#define WINDROSE_HPP_

#include <QObject>
#include <QColor>
#include <QPainter>
#include <QQuickPaintedItem>

class Windrose : public QQuickPaintedItem
{
        Q_OBJECT
        Q_PROPERTY(int windDirection READ windDirection WRITE setWindDirection NOTIFY windDirectionChanged)
        Q_PROPERTY(int segmentCount READ segmentCount WRITE setSegmentCount NOTIFY segmentCountChanged)
        Q_PROPERTY(int strokeWidth READ strokeWidth WRITE setStrokeWidth NOTIFY strokeWidthChanged)
        Q_PROPERTY(int gapAngle READ gapAngle WRITE setGapAngle NOTIFY gapAngleChanged)
        Q_PROPERTY(int ringSpace READ ringSpace WRITE setRingSpace NOTIFY ringSpaceChanged)
        Q_PROPERTY(QColor backgroundColor READ backgroundColor WRITE setBackgroundColor NOTIFY backgroundColorChanged)
        Q_PROPERTY(QColor normColor READ normColor WRITE setNormColor NOTIFY normColorChanged)
        Q_PROPERTY(QColor highlightColor READ highlightColor WRITE setHighlightColor NOTIFY highlightColorChanged)

public:
        explicit Windrose(QQuickItem * parent = nullptr);

        void paint(QPainter * painter) override;

        int windDirection() const { return mWindDirection; }
        int segmentCount() const { return mSegmentCount; }
        int strokeWidth() const { return mStrokeWidth; }
        int gapAngle() const { return mGapAngle; }
        int ringSpace() const { return mRingSpace; }
        QColor backgroundColor() const { return mBackgroundColor; }
        QColor normColor() const { return mNormColor; }
        QColor highlightColor() const { return mHighlightColor; }

        void setWindDirection(int dir);
        void setSegmentCount(int cnt);
        void setStrokeWidth(int w);
        void setGapAngle(int ang);
        void setRingSpace(int s);
        void setBackgroundColor(const QColor &c);
        void setNormColor(const QColor &c);
        void setHighlightColor(const QColor &c);

signals:
        void windDirectionChanged();
        void segmentCountChanged();
        void strokeWidthChanged();
        void gapAngleChanged();
        void ringSpaceChanged();
        void backgroundColorChanged();
        void normColorChanged();
        void highlightColorChanged();

private:
        int mWindDirection = 0;
        int mSegmentCount = 20;
        int mStrokeWidth = 16;
        int mGapAngle = 4;
        int mRingSpace = 16;
        QColor mBackgroundColor;
        QColor mNormColor;
        QColor mHighlightColor;
};

#endif // WINDROSE_HPP_