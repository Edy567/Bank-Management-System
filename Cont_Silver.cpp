#include "Cont_Silver.h"

#include <utility>


ContSilver::ContSilver(const std::vector<Card> &carduri, std::string IBAN,
                       const std::vector<Tranzactie> &tranzactii) : Cont(carduri, std::move(IBAN), tranzactii, 3) {
    std::cout << " (Cont silver) creat";
}

ContSilver *ContSilver::clone() const { return new ContSilver(*this); }

double ContSilver::calculeazaComision(const double suma) const {
    return suma * 0.01;
}

void ContSilver::afisareDetaliata(std::ostream &os) const {
    os << "Max 3 carduri, 1% comision.";
}
