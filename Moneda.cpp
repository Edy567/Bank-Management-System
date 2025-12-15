#include "Moneda.h"
#include <utility>

Moneda::Moneda(std::string cod, std::string nume, const double &cursValutar)
    : cod{std::move(cod)}, nume{std::move(nume)}, cursValutar{cursValutar} {
    std::cout << "(Moneda) Constructor parametrizat\n";
}

Moneda::Moneda(const Moneda &other)
    : cod{other.cod}, nume{other.nume}, cursValutar{other.cursValutar} {
    std::cout << "(Moneda) Constructor de copiere\n";
}

Moneda &Moneda::operator=(const Moneda &other) {
    if (this != &other) {
        cod = other.cod;
        nume = other.nume;
        cursValutar = other.cursValutar;
        std::cout << "(Moneda) Operator= apelat\n";
    }
    return *this;
}

Moneda::~Moneda() {
    std::cout << "(Moneda) Destructor\n";
}

std::ostream &operator<<(std::ostream &os, const Moneda &moneda) {
    os << moneda.cod << " " << moneda.nume << " " << moneda.cursValutar;
    return os;
}