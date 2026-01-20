#ifndef CONT_FACTORY_H
#define CONT_FACTORY_H

#include "Cont.h"
#include <string>
#include <vector>

class ContFactory {
public:
    // Metoda statica principala
    static Cont *creareCont(const std::string &tipCont,
                            const std::vector<Card> &carduri,
                            const std::string &iban,
                            const std::vector<Tranzactie> &tranzactii);

private:
    // Functii ajutatoare private (logica noua pt procentaj)
    static bool valideazaIBAN(const std::string &iban);

    static std::string toUpper(std::string str);
};

#endif
