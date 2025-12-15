#include "Tranzactie.h"
#include <utility>

int Tranzactie::nrTranzactii = 0;

Tranzactie::Tranzactie(const int &sum, std::string descriere)
    : descriere{std::move(descriere)}, suma{sum} {
    std::cout << "(Tranzactie) Constructor parametrizat\n";
    Tranzactie::nrTranzactii++;
}

Tranzactie::Tranzactie(const Tranzactie &tr)
    : descriere{tr.descriere}, suma{tr.suma} {
    std::cout << "(Tranzactie) Constructor de copiere\n";
    Tranzactie::nrTranzactii++;
}

Tranzactie &Tranzactie::operator=(const Tranzactie &tr) {
    if (this != &tr) {
        suma = tr.suma;
        descriere = tr.descriere;
        std::cout << "(Tranzactie) Operator= apelat\n";
    }
    return *this;
}

Tranzactie::~Tranzactie() {
    std::cout << "(Tranzactie) Destructor\n";
}

std::ostream &operator<<(std::ostream &os, const Tranzactie &tr) {
    return os << tr.descriere << " " << tr.suma;
}
