#ifndef MYCRC_H
#define MYCRC_H

#include <QByteArray>
#include <QDebug>
#include "macro.h"

#ifdef USED_CRYPTO
    #include "qaesencryption.h"
#endif

class MyCrc
{
public:
    MyCrc();
    ~MyCrc();

public:
    static uint8_t my_crc8_check(uint8_t *data,uint8_t offset,uint16_t len);

    static uint16_t my_crc16_check(uint8_t *data,uint8_t offset,uint16_t len);

#ifdef USED_CRYPTO
    static QByteArray m_key;
    static QAESEncryption m_Crypto;

    static const QByteArray& encodedText(const QByteArray& data);

    static const QByteArray& decodedText(const QByteArray& data);
#endif

};

#endif // MYCRC_H
