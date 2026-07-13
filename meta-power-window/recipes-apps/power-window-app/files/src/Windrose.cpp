#include <QObject>
#include <QtMath>
#include <QColor>
#include <QPainter>
#include <QQuickPaintedItem>
#include "Windrose.hpp"

Windrose::Windrose(QQuickItem * parent) : QQuickPaintedItem(parent) {}

void Windrose::paint(QPainter * painter)
{
        painter->setRenderHint(QPainter::Antialiasing);

        const qreal cx = width() / 2.0;
        const qreal cy = height() / 2.0;
        const qreal r = (qMin(width(), height()) - mStrokeWidth) / 2.0 - mRingSpace;
        const qreal outerR = r + mStrokeWidth / 2.0 + mRingSpace;
        const qreal innerR = r - mStrokeWidth / 2.0 - mRingSpace;
        const qreal sliceAngle = 360.0 / mSegmentCount;
        const int actSeg = (mSegmentCount > 0)
                ? qRound(mWindDirection / sliceAngle) % mSegmentCount
                : -1;
        const QRectF arcRect(cx - r, cy - r, r * 2, r * 2);

        // background
        painter->setPen(Qt::NoPen);
        painter->setBrush(mBackgroundColor);
        painter->drawEllipse(QRectF(cx - outerR, cy - outerR, outerR * 2, outerR * 2));

        // segments
        for (int i = 0; i < mSegmentCount; i++) {
                const qreal startDeg = i * sliceAngle + mGapAngle / 2.0 - 90.0;
                const qreal sweepDeg = sliceAngle - mGapAngle;
                QPen pen;
                pen.setWidthF(mStrokeWidth);
                pen.setCapStyle(Qt::FlatCap);
                pen.setColor((i == actSeg) ? mHighlightColor : mNormColor);
                painter->setPen(pen);
                painter->drawArc(arcRect, static_cast<int>(-startDeg * 16), static_cast<int>(-sweepDeg * 16));
        }

        // inner ring
        QPen ringPen(mHighlightColor, 2);
        painter->setPen(ringPen);
        painter->setBrush(Qt::NoBrush);
        painter->drawEllipse(QRectF(cx - innerR, cy - innerR, innerR * 2, innerR * 2));
        // outer ring
        painter->drawEllipse(QRectF(cx - outerR, cy - outerR, outerR * 2, outerR * 2));
}

void Windrose::setWindDirection(int dir)
{
        if (mWindDirection == dir) { return; }
        mWindDirection = dir;
        emit windDirectionChanged();
        update();
}

void Windrose::setSegmentCount(int cnt)
{
        if (mSegmentCount == cnt) { return; }
        mSegmentCount = cnt;
        emit segmentCountChanged();
        update();
}

void Windrose::setStrokeWidth(int w)
{
        if (mStrokeWidth == w) { return; }
        mStrokeWidth = w;
        emit strokeWidthChanged();
        update();
}

void Windrose::setGapAngle(int ang)
{
        if (mGapAngle == ang) { return; }
        mGapAngle = ang;
        emit gapAngleChanged();
        update();
}

void Windrose::setRingSpace(int s)
{
        if (mRingSpace == s) { return; }
        mRingSpace = s;
        emit ringSpaceChanged();
        update();
}

void Windrose::setBackgroundColor(const QColor &c)
{
        if (mBackgroundColor == c) { return; }
        mBackgroundColor = c;
        emit backgroundColorChanged();
        update();
}

void Windrose::setNormColor(const QColor &c)
{
        if (mNormColor == c) { return; }
        mNormColor = c;
        emit normColorChanged();
        update();
}

void Windrose::setHighlightColor(const QColor &c)
{
        if (mHighlightColor == c) { return; }
        mHighlightColor = c;
        emit highlightColorChanged();
        update();
}

