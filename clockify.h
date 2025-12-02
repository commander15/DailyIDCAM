#ifndef CLOCKIFY_H
#define CLOCKIFY_H

#include <QObject>
#include <QNetworkAccessManager>

class Clockify : public QObject
{
    Q_OBJECT

public:
    Clockify(QObject *parent = nullptr);

    void getReport(const QString &workspaceId, const std::function<void (QIODevice *)> &callback);

    void setToken(const QString &token)
    { m_token = token.toLatin1(); }

signals:
    void errorOccured(const QString &description);

private:
    static QByteArray generateExportBody(const QDate &date);

    QByteArray m_token;
    QNetworkAccessManager m_netMan;
};

#endif // CLOCKIFY_H
