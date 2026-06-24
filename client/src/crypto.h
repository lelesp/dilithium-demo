#ifndef CRYPTO_H
#define CRYPTO_H

#include <QByteArray>
#include <QString>

class Crypto {
public:
    // Default parameter set, used only for the startup log line.
    static const QString ALGORITHM;

    // Determine the ML-DSA parameter set from the raw signature length (in
    // bytes). FIPS 204 signature sizes are unique per parameter set, so the
    // algorithm can be recovered from a package without extra metadata.
    // Returns an empty string if the length matches no known parameter set.
    static QString detectAlgorithm(int signatureBytesLength);

    // Verifies a signature, auto-detecting the ML-DSA parameter set from the
    // signature length. publicKey and signature are hex-encoded ASCII; message
    // is raw bytes.
    static bool verifySignature(const QByteArray& publicKey,
                                const QByteArray& message,
                                const QByteArray& signature);

    static QByteArray hexToBytes(const QString& hex);
    static QString bytesToHex(const QByteArray& bytes);
};

#endif
