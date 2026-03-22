#ifndef CRYPTO_H
#define CRYPTO_H

#include <QByteArray>
#include <QString>

class Crypto {
public:
    static const QString ALGORITHM;
    
    static bool verifySignature(const QByteArray& publicKey,
                                const QByteArray& message,
                                const QByteArray& signature);
    
    static QByteArray hexToBytes(const QString& hex);
    static QString bytesToHex(const QByteArray& bytes);
};

#endif
