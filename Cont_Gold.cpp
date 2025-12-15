//
// Created by User on 12/2/2025.
//
#include "Cont_Gold.h"
#include "utility"

ContGold::ContGold(const std::vector<Card> &carduri, std::string IBAN, const std::vector<Tranzactie> &tranzactii) :Cont(carduri, std::move(IBAN), tranzactii, 5) {
    std::cout<<" (Cont silver) creat";
}
ContGold* ContGold::clone() const { return new ContGold(*this); }

double ContGold::calculeazaComision(double suma) const {
    return suma * 0.005;
}

void ContGold::afisareDetaliata(std::ostream &os) const {
    os << "Max 5 carduri, 0.005% comision.";
}