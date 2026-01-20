//
// Created by User on 1/20/2026.
//

#include "ContFactory.h"
#include "Cont_Silver.h"
#include "Cont_Gold.h"
#include "Cont_premium.h"
#include "ContStudent.h"
#include <algorithm>
#include <iostream>

Cont* ContFactory::creareCont(const std::string &tipCont,
                              const std::vector<Card>& carduri,
                              const std::string& iban,
                              const std::vector<Tranzactie>& tranzactii) {


    if (!valideazaIBAN(iban)) {
        std::cerr << "Eroare : IBAN invalid detectat -> " << iban << ". Se anuleaza crearea contului.\n";
        return nullptr;

    }


    std::string tip = toUpper(tipCont);


    if (tip == "SILVER") {
        return new ContSilver(carduri, iban, tranzactii);
    }
    if (tip == "GOLD") {
        return new ContGold(carduri, iban, tranzactii);
    }
     if (tip == "PREMIUM") {
        return new ContPremium(carduri, iban, tranzactii);
    }
     if (tip == "STUDENT") {
        return new ContStudent(carduri, iban, tranzactii);
    }

    std::cerr << "Eroare Factory: Tip cont necunoscut -> " << tip << "\n";
    return nullptr;
}

bool ContFactory::valideazaIBAN(const std::string& iban) {

    if (iban.length() < 10) return false;
    if (iban.substr(0, 2) != "RO") return false;
    return true;
}

std::string ContFactory::toUpper(std::string str) {
    std::ranges::transform(str, str.begin(), ::toupper);
    return str;
}