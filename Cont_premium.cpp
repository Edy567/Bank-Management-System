//
// Created by User on 12/2/2025.
//

#include "Cont_premium.h"


ContPremium::ContPremium(const std::vector<Card> &carduri, std::string IBAN,
                         const std::vector<Tranzactie> &tranzactii) : Cont(carduri, std::move(IBAN), tranzactii, 8) {
    std::cout << " (Cont silver) creat";
}

ContPremium *ContPremium::clone() const { return new ContPremium(*this); }

double ContPremium::calculeazaComision(const double suma) const {
    return suma * 0.0;
}

void ContPremium::afisareDetaliata(std::ostream &os) const {
    os << "Max 8 carduri, fara comision.";
}
