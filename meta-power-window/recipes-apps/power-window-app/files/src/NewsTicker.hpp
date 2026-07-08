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
        Q_PROPERTY(QStringList feedNames READ feedNames CONSTANT)
        Q_PROPERTY(QString currentFeed READ currentFeed WRITE setCurrentFeed NOTIFY currentFeedChanged)

public:
        explicit NewsTicker(QObject * parent = nullptr);
        void start();
        QVariantList headlines() const;
        bool ready() const;
        QString feedUrl() const;
        void setFeedUrl(const QString &url);
        QStringList feedNames() const;
        QString currentFeed() const;

public slots:
        void setCurrentFeed(const QString &name);

signals:
        void headlinesChanged();
        void feedUrlChanged();
        void currentFeedChanged();

private:
        void fetch();
        void parse(const QByteArray &xml);
        QNetworkAccessManager * mNet;
        QTimer * mRefreshTimer;
        QString mFeedUrl;
        QVariantList mHeadlines;
        QString mCurrentImageUrl;
        const QMap<QString, QString> mFeedUrls;
        QString mCurrentFeed;
        bool mReady;
};

#endif // NEWS_TICKER_HPP_