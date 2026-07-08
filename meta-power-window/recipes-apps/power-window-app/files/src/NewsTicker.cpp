#include <QObject>
#include <QTimer>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QVariantList>
#include <QXmlStreamReader>
#include <QSettings>
#include "NewsTicker.hpp"

#define NT_SET_NAME "news/feedUrl"
#define NT_DEF_URL  "https://www.dnn.de/arc/outboundfeeds/rss/"
#define NT_RFS_DEL  (30 * 60 * 1000)

NewsTicker::NewsTicker(QObject * parent) : QObject(parent), mReady(false)
{
        mNet = new QNetworkAccessManager(this);
        QSettings settings;
        mFeedUrl = settings.value(NT_SET_NAME, NT_DEF_URL).toString();
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
        settings.setValue(NT_SET_NAME, url);
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
                        }
                } else if (sr.isEndElement() && sr.name().toString() == "item") {
                        if (!title.isEmpty()) {
                                QVariantMap item;
                                item["title"] = title;
                                item["description"] = description;
                                mHeadlines.append(item);
                        }
                }
        }
        if (!mHeadlines.isEmpty()) {
                mReady = true;
                emit headlinesChanged();
        }
}