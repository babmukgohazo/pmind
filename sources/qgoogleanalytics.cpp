#include <QtNetwork/QHostInfo>
#include <QCoreApplication>
#include <QByteArray>
#include <QUrlQuery>
#include <QString>
#include <QUuid>
#include <QDebug>
#include "headers/qgoogleanalytics.h"


QGoogleAnalytics::QGoogleAnalytics(QObject *parent){
    qDebug() << "QGoogleAnalytics::init()" << QCoreApplication::applicationName();

    nam = new QNetworkAccessManager();

    // Call a slot when network request is sent.
    connect(nam, SIGNAL(finished(QNetworkReply*)), this, SLOT(finished(QNetworkReply*)));

   _settings = new QSettings(QSettings::IniFormat, QSettings::UserScope, "babmukgohazo","PPoMIND");
   uuid = _settings->value("uuid", "").toString();
   if (uuid.isEmpty()) {
           uuid = QUuid::createUuid().toString().replace("{", "").replace("}", "").replace("-", "");
           _settings->setValue("uuid", uuid);
       }
}

void QGoogleAnalytics::sendEvent(QString category, QString action, QString label, int value){
    // Get hostname so we can set it as a parameter.
    QString hostname = QHostInfo::localHostName() + "." + QHostInfo::localDomainName();

    // Create an http network request
    QNetworkRequest req(QUrl("http://www.google-analytics.com/collect"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Build up the query parameters.
    QUrlQuery query;
    query.addQueryItem("v", "1"); // Version
    query.addQueryItem("tid", "UA-88385465-1"); // Tracking ID - use value assigned to you by Google Analytics
    query.addQueryItem("cid", uuid); // Client ID

    query.addQueryItem("t", "event"); // Event hit type
    query.addQueryItem("ec", category); // Event category
    query.addQueryItem("ea", action); // Event action
    query.addQueryItem("el", label); // Event label
    query.addQueryItem("ev", QString::number(value)); // Event value

    QByteArray data;
    data.append(query.query());
    qDebug() << data << endl; // Output for debug purposes.
    qDebug() << "Request headers: ";
    QList<QByteArray> reqHeaders = req.rawHeaderList();
    foreach( QByteArray reqName, reqHeaders )
    {
    QByteArray reqValue = req.rawHeader( reqName );
    qDebug() << reqName << ": " << reqValue;
    }

    nam->post(req, data);
}

void QGoogleAnalytics::sendPageView(QString pageName){
    // Get hostname so we can set it as a parameter.
    QString hostname = QHostInfo::localHostName() + "." + QHostInfo::localDomainName();

    // Create an http network request
    QNetworkRequest req(QUrl("http://www.google-analytics.com/collect"));
    req.setHeader(QNetworkRequest::ContentTypeHeader, "application/x-www-form-urlencoded");

    // Build up the query parameters.
    QUrlQuery query;
    query.addQueryItem("v", "1"); // Version
    query.addQueryItem("tid", "UA-88385465-1"); // Tracking ID - use value assigned to you by Google Analytics
    query.addQueryItem("cid", uuid); // Client ID

    query.addQueryItem("t", "pageview"); // Pageview hit type
    query.addQueryItem("dh", hostname); // Document hostname
    query.addQueryItem("dp", pageName);//ui->pageViewPageLineEdit->text()); // Page
    query.addQueryItem("dt", "mindmapview");//ui->pageViewTitleLineEdit->text()); // Title
    query.addQueryItem("an", "PPoMIND");
    query.addQueryItem("av", "1.0");

    QByteArray data;
    data.append(query.query());
    qDebug() << data << endl; // Output for debug purposes.
    qDebug() << "Request headers: ";
    QList<QByteArray> reqHeaders = req.rawHeaderList();
    foreach( QByteArray reqName, reqHeaders )
    {
    QByteArray reqValue = req.rawHeader( reqName );
    qDebug() << reqName << ": " << reqValue;
    }

    nam->post(req, data);
}

void QGoogleAnalytics::finished(QNetworkReply *reply)
{
    // Do some basic error checking.

    if (reply->error() == QNetworkReply::NoError) {
        qDebug() << "Finished, no error";
    } else {
        qDebug() << "Finished, error:" << reply->error();
    }
}
