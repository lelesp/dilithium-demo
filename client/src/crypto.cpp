#include "crypto.h"

#include <oqs/oqs.h>
#include <QDebug>
#include <vector>
#include <cstring>

const QString Crypto::ALGORITHM = "ML-DSA-65";

namespace {
// FIPS 204 signature size (bytes) -> liboqs algorithm identifier. Each
// parameter set has a distinct signature length, so the algorithm used to
// produce a package can be recovered without extra metadata.
struct AlgoBySize {
    int signatureBytes;
    const char* name;
};
constexpr AlgoBySize kAlgoBySize[] = {
    {2420, "ML-DSA-44"},
    {3309, "ML-DSA-65"},
    {4627, "ML-DSA-87"},
};
}

QString Crypto::detectAlgorithm(int signatureBytesLength) {
    for (const auto& entry : kAlgoBySize) {
        if (entry.signatureBytes == signatureBytesLength) {
            return QString::fromLatin1(entry.name);
        }
    }
    return QString();
}

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

    const QString algorithm = detectAlgorithm(sigBytes.size());
    if (algorithm.isEmpty()) {
        qWarning() << "Unknown ML-DSA signature size:" << sigBytes.size()
                   << "bytes; cannot determine algorithm";
        return false;
    }

    OQS_SIG* sig_ctx = OQS_SIG_new(algorithm.toLatin1().constData());
    if (!sig_ctx) {
        qWarning() << "Failed to create signature object for" << algorithm;
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
