#include "apiclient.h"

#include <QNetworkRequest>
#include <QHttpMultiPart>
#include <QFile>
#include <QFileInfo>
#include <QUrl>
#include <QHttpPart>

ApiClient::ApiClient(const QString& baseUrl, QObject* parent)
    : QObject(parent)
    , m_manager(new QNetworkAccessManager(this))
    , m_baseUrl(baseUrl)
{
}

void ApiClient::signFile(const QString& filePath, const QString& algorithm) {
    QUrl url(m_baseUrl + "/api/sign");
    QNetworkRequest request(url);

    QHttpMultiPart* multiPart = new QHttpMultiPart(QHttpMultiPart::FormDataType);

    QFile* file = new QFile(filePath);
    if (!file->open(QIODevice::ReadOnly)) {
        emit errorOccurred("Cannot open file: " + filePath);
        delete multiPart;
        return;
    }

    QHttpPart algorithmPart;
    algorithmPart.setHeader(QNetworkRequest::ContentDispositionHeader,
                            QVariant("form-data; name=\"algorithm\""));
    algorithmPart.setBody(algorithm.toUtf8());
    multiPart->append(algorithmPart);

    QHttpPart filePart;
    QFileInfo fileInfo(filePath);
    QString fileName = fileInfo.fileName();
    filePart.setHeader(QNetworkRequest::ContentDispositionHeader,
                       QString("form-data; name=\"file\"; filename=\"%1\"").arg(fileName));
    filePart.setBodyDevice(file);
    multiPart->append(filePart);
    
    file->setParent(multiPart);
    
    QNetworkReply* reply = m_manager->post(request, multiPart);
    multiPart->setParent(reply);
    
    connect(reply, &QNetworkReply::finished, this, &ApiClient::onSignFileReply);
}

void ApiClient::onSignFileReply() {
    QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());
    if (!reply) return;
    
    if (reply->error() != QNetworkReply::NoError) {
        emit errorOccurred("Sign file error: " + reply->errorString());
        reply->deleteLater();
        return;
    }
    
    QByteArray zipData = reply->readAll();
    emit fileSigned(zipData);
    
    reply->deleteLater();
}
