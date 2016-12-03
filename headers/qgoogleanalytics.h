#ifndef QGOOGLEANALYTICS_H
#define QGOOGLEANALYTICS_H

#include <QObject>
#include <QSettings>
#include <QtNetwork/QNetworkAccessManager>
#include <QtNetwork/QNetworkReply>
#include <QUuid>

class QGoogleAnalytics : public QObject
{
    Q_OBJECT
public:
    explicit QGoogleAnalytics(QObject *parent = 0);

signals:

public slots:
    void sendEvent(QString category, QString action, QString label, int value = 0);
    void sendPageView(QString pageName);
    void finished(QNetworkReply *reply);

private:
    QNetworkAccessManager *nam;
    QString uuid;
    QSettings *_settings;
};

#endif // QGOOGLEANALYTICS_H


