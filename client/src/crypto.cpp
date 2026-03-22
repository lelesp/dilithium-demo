#include "crypto.h"

#include <oqs/oqs.h>
#include <QDebug>
#include <vector>
#include <cstring>

const QString Crypto::ALGORITHM = "ML-DSA-65";

QByteArray Crypto::hexToBytes(const QString& hex) {
    QByteArray bytes;
    QString cleanHex = hex;
    cleanHex.remove(" ");
    
    for (int i = 0; i < cleanHex.length(); i += 2) {
        QString byteStr = cleanHex.mid(i, 2);
        bool ok;
        int byte = byteStr.toInt(&ok, 16);
        if (ok) {
            bytes.append(static_cast<char>(byte));
        }
    }
    return bytes;
}

QString Crypto::bytesToHex(const QByteArray& bytes) {
    QString hex;
    for (char c : bytes) {
        hex += QString("%1").arg(static_cast<unsigned char>(c), 2, 16, QChar('0'));
    }
    return hex;
}

bool Crypto::verifySignature(const QByteArray& publicKey,
                            const QByteArray& message,
                            const QByteArray& signature) {
    QByteArray pkBytes = hexToBytes(QString::fromLatin1(publicKey));
    QByteArray sigBytes = hexToBytes(QString::fromLatin1(signature));
    
    OQS_SIG* sig_ctx = OQS_SIG_new("ML-DSA-65");
    if (!sig_ctx) {
        qWarning() << "Failed to create signature object for ML-DSA-65";
        return false;
    }
    
    std::vector<uint8_t> pk_vec(pkBytes.begin(), pkBytes.end());
    std::vector<uint8_t> msg_vec(message.begin(), message.end());
    std::vector<uint8_t> sig_vec(sigBytes.begin(), sigBytes.end());
    
    OQS_STATUS status = OQS_SIG_verify(sig_ctx, msg_vec.data(), msg_vec.size(), sig_vec.data(), sig_vec.size(), pk_vec.data());
    bool result = (status == OQS_SUCCESS);
    
    OQS_SIG_free(sig_ctx);
    
    return result;
}
