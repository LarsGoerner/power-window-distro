#ifndef NEWS_TICKER_HPP_
#define NEWS_TICKER_HPP_

#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QVariantList>

class NewsTicker : public QObject
{
        Q_OBJECT
        Q_PROPERTY(QVariantList headlines READ headlines NOTIFY headlinesChanged)
        Q_PROPERTY(bool ready READ ready NOTIFY headlinesChanged)
        Q_PROPERTY(QString feedUrl READ feedUrl WRITE setFeedUrl NOTIFY feedUrlChanged)

public:
        explicit NewsTicker(QObject * parent = nullptr);
        void start();
        QVariantList headlines() const;
        bool ready() const;
        QString feedUrl() const;
        void setFeedUrl(const QString &url);

signals:
        void headlinesChanged();
        void feedUrlChanged();

private:
        void fetch();
        void parse(const QByteArray &xml);
        QNetworkAccessManager * mNet;
        QTimer * mRefreshTimer;
        QString mFeedUrl;
        QVariantList mHeadlines;
        QString mCurrentImageUrl;
        bool mReady;
};

#endif // NEWS_TICKER_HPP_