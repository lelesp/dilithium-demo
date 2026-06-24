#ifndef APICLIENT_H
#define APICLIENT_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QByteArray>
#include <QString>

class ApiClient : public QObject {
    Q_OBJECT

public:
    explicit ApiClient(const QString& baseUrl, QObject* parent = nullptr);
    
    void signFile(const QString& filePath, const QString& algorithm);
    
signals:
    void fileSigned(const QByteArray& zipData);
    void errorOccurred(const QString& error);

private slots:
    void onSignFileReply();

private:
    QNetworkAccessManager* m_manager;
    QString m_baseUrl;
};

#endif
