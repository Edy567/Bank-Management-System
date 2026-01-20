#ifndef CARD_VALIDATOR_H
#define CARD_VALIDATOR_H

#include <string>

class CardValidator {
public:
    static bool verificaLuhn(const std::string &numarCard);


    static bool verificaExpirare(const std::string &dataExp);
};

#endif
