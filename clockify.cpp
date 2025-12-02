#include "clockify.h"

#include <QNetworkRequest>
#include <QNetworkReply>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>
#include <QTemporaryFile>

Clockify::Clockify(QObject *parent)
    : QObject(parent)
{}

void Clockify::getReport(const QString &workspaceId, const std::function<void(QIODevice *device)> &callback)
{
    const QString url("https://reports.api.clockify.me/v1/workspaces/%1/reports/detailed?exportType=XLSX");

    QNetworkRequest request(QUrl(url.arg(workspaceId)));
    request.setRawHeader("Accept", "application/json");
    request.setRawHeader("Content-Type", "application/json");
    request.setRawHeader("X-Api-Key", m_token);

    const QByteArray body = generateExportBody(QDate::currentDate());

    QNetworkReply *reply = m_netMan.post(request, body);

    connect(reply, &QNetworkReply::finished, this, [this, reply, callback]() {
        const int httpStatus = reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
        if (httpStatus != 200) {
            // An error ? We catch it there
            QTextStream(stderr) << "HTTP " << httpStatus << " error occured" << Qt::endl;
            callback(nullptr);
            return;
        }

        QTemporaryFile *file = new QTemporaryFile(this);
        if (file->open()) {
            file->write(reply->readAll());
            file->flush();
            file->reset();
            callback(file);
        } else {
            callback(nullptr);
        }

        reply->deleteLater();
    });
}

QByteArray Clockify::generateExportBody(const QDate &date)
{
    const QDateTime start(date, QTime());
    const QDateTime end(date, QTime(23, 59, 59));

    QFile f(":/resources/export_body.json");
    f.open(QIODevice::ReadOnly);

    QByteArray body = f.readAll();
    body.replace("{START_DATE}", start.toString(Qt::ISODate).toLatin1());
    body.replace("{END_DATE}", end.toString(Qt::ISODate).toLatin1());
    return body;
}
