#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVariantList>
#include <QXmlStreamReader>
#include <QSettings>
#include "NewsTicker.hpp"

#define SET_PATH          "NewsTicker/"
#define SET_FEED_URL_PATH SET_PATH "feedUrl"
#define NT_RFS_DEL  (30 * 60 * 1000)

NewsTicker::NewsTicker(QObject * parent) : QObject(parent), mReady(false)
        , mFeedUrls({
                {"DNN", "https://www.dnn.de/arc/outboundfeeds/rss/"},
                {"Sächsische", "https://www.saechsische.de/arc/outboundfeeds/rss/"},
                {"Tagesschau", "https://www.tagesschau.de/infoservices/alle-meldungen-100~rss2.xml"},
                {"FAZ", "https://www.faz.net/rss/aktuell/"}
        })
{
        mNet = new QNetworkAccessManager(this);
        QSettings settings;
        QString saved = settings.value(SET_FEED_URL_PATH, "").toString();
        for (auto it = mFeedUrls.constBegin(); it != mFeedUrls.constEnd(); it++) {
                if (it.value() == saved) {
                        mCurrentFeed = it.key();
                        break;
                }
        }
        if (mCurrentFeed.isEmpty()) {
                mCurrentFeed = mFeedUrls.firstKey();
                saved = mFeedUrls.first();
        }
        mFeedUrl = saved;
        mRefreshTimer = new QTimer(this);
        mRefreshTimer->setInterval(NT_RFS_DEL);
        connect(mRefreshTimer, &QTimer::timeout, this, &NewsTicker::fetch);
}

void NewsTicker::start()
{
        fetch();
        mRefreshTimer->start();
}

QVariantList NewsTicker::headlines() const { return mHeadlines; }

bool NewsTicker::ready() const { return mReady; }

QString NewsTicker::feedUrl() const { return mFeedUrl; }

void NewsTicker::setFeedUrl(const QString &url)
{
        if (url.isEmpty() || url == mFeedUrl) { return; }
        mFeedUrl = url;
        QSettings settings;
        settings.setValue(SET_FEED_URL_PATH, url);
        emit feedUrlChanged();
        mHeadlines.clear();
        mReady = false;
        emit headlinesChanged();
        fetch();
}

QStringList NewsTicker::feedNames() const { return mFeedUrls.keys(); }

QString NewsTicker::currentFeed() const { return mCurrentFeed; }

void NewsTicker::setCurrentFeed(const QString &name)
{
        if (name == mCurrentFeed ||!mFeedUrls.contains(name)) { return; }
        mCurrentFeed = name;
        mFeedUrl = mFeedUrls[name];
        QSettings settings;
        settings.setValue(SET_FEED_URL_PATH, mFeedUrl);
        emit currentFeedChanged();
        emit feedUrlChanged();
        mHeadlines.clear();
        mReady = false;
        emit headlinesChanged();
        fetch();
}

void NewsTicker::fetch()
{
        QNetworkReply * reply = mNet->get(QNetworkRequest(QUrl(mFeedUrl)));
        connect(reply, &QNetworkReply::finished, this, [this, reply] {
                reply->deleteLater();
                if (reply->error() != QNetworkReply::NoError) { return; }
                parse(reply->readAll());
        });
}

void NewsTicker::parse(const QByteArray &xml)
{
        mHeadlines.clear();
        QXmlStreamReader sr(xml);
        QString title;
        QString description;
        while (!sr.atEnd() && !sr.hasError()) {
                sr.readNext();
                if (sr.isStartElement()) {
                        if (sr.name().toString() == "item") {
                                title.clear();
                                description.clear();
                        } else if (sr.name().toString() == "title") {
                                title = sr.readElementText().trimmed();
                        } else if (sr.name().toString() == "description") {
                                description = sr.readElementText().trimmed();
                        } else if (sr.namespaceUri().toString() == "http://search.yahoo.com/mrss/"
                                   && sr.name().toString() == "content") {
                                QString url = sr.attributes().value("url").toString();
                                if (url.startsWith("https://")) { mCurrentImageUrl = url; }
                        }
                } else if (sr.isEndElement() && sr.name().toString() == "item") {
                        if (!title.isEmpty()) {
                                QVariantMap item;
                                item["title"] = title;
                                item["description"] = description;
                                if (!mCurrentImageUrl.isEmpty()) { item["imageUrl"] = mCurrentImageUrl; }
                                mCurrentImageUrl.clear();
                                mHeadlines.append(item);
                        }
                }
        }
        if (!mHeadlines.isEmpty()) {
                mReady = true;
                emit headlinesChanged();
        }
}