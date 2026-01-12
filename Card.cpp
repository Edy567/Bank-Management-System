#include "Card.h"
#include <utility>

Card::Card(const double &suma, std::string titular, std::string numarExp,
           std::string nrCard, const Moneda &moneda)
    : suma{suma}, titular{std::move(titular)}, numarExp{std::move(numarExp)}, nrCard{std::move(nrCard)},
      moneda{moneda} {
    std::cout << "(Card) Constructor parametrizat\n";
}

Card::Card(const Card &other)
    : suma{other.suma}, titular{other.titular}, numarExp{other.numarExp},
      nrCard{other.nrCard}, moneda{other.moneda} {
    std::cout << "(Card) Constructor de copiere\n";
}

Card &Card::operator=(const Card &other) {
    if (this != &other) {
        suma = other.suma;
        titular = other.titular;
        numarExp = other.numarExp;
        nrCard = other.nrCard;
        moneda = other.moneda;
        std::cout << "(Card) Operator= apelat\n";
    }
    return *this;
}

Card::~Card() {
    std::cout << "(Card) Destructor\n";
}

std::ostream &operator<<(std::ostream &os, const Card &card) {
    os << card.titular << " " << card.numarExp << " " << card.nrCard << " \n " << card.moneda;
    return os;
}

double Card::getSuma() const { return suma; }
const Moneda &Card::getMoneda() const { return moneda; }
const std::string &Card::getTitular() const { return titular; }
const std::string &Card::getNrCard() const { return nrCard; }

void Card::scadeSuma(double valoare) { suma -= valoare; }
void Card::adaugaSuma(double valoare) { suma += valoare; }