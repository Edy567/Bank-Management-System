#ifndef CONT_FACTORY_H
#define CONT_FACTORY_H

#include "Cont.h"
#include <string>
#include <vector>

class ContFactory {
public:
    static Cont *creareCont(const std::string &tipCont,
                            const std::vector<Card> &carduri,
                            const std::string &iban,
                            const std::vector<Tranzactie> &tranzactii);

private:
    static bool valideazaIBAN(const std::string &iban);

    static std::string toUpper(std::string str);
};

#endif
