#include <iostream>

#include <crypto++/sha.h>
#include <crypto++/filters.h>
#include <crypto++/base64.h>
#include <crypto++/hex.h>

#include "sha256.h"

QString sha256hash(const QString &msg)
{
    CryptoPP::SHA256 hash;
    byte digest[CryptoPP::SHA256::DIGESTSIZE];

    hash.CalculateDigest(digest, (byte*) msg.toStdString().c_str(), msg.length());

    CryptoPP::HexEncoder encoder;
    std::string output;
    encoder.Attach(new CryptoPP::StringSink(output));
    encoder.Put(digest, sizeof(digest));
    encoder.MessageEnd();

    return QString::fromStdString(output);
}
