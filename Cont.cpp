#include "Cont.h"
#include <utility>

Cont::Cont(const std::vector<Card> &carduri, std::string IBAN,
           const std::vector<Tranzactie> &tranzactii, int limitaCarduri)
    : IBAN{std::move(IBAN)}, carduri{carduri}, tranzactii{tranzactii}, limitaCarduri{limitaCarduri} {
    std::cout << "(Cont) Constructor parametrizat\n";
}

Cont::Cont(const Cont &other)
    : IBAN{other.IBAN}, carduri{other.carduri}, tranzactii{other.tranzactii}, limitaCarduri{other.limitaCarduri} {
    std::cout << "(Cont) Constructor de copiere\n";
}

Cont &Cont::operator=(const Cont &other) {
    if (this != &other) {
        IBAN = other.IBAN;
        carduri = other.carduri;
        tranzactii = other.tranzactii;
        limitaCarduri = other.limitaCarduri;
        std::cout << "(Cont) Operator= apelat\n";
    }
    return *this;
}

Cont::~Cont() {
    std::cout << "(Cont) Destructor\n";
}

std::ostream &operator<<(std::ostream &os, const Cont &cont) {
    os << cont.IBAN << " \n";
    for (const auto &card: cont.carduri) {
        os << card << " ";
    }
    os << " \n ";
    for (const auto &tr: cont.tranzactii) {
        os << tr << " " << "\n";
    }
    return os;
}

const std::string &Cont::getIBAN() const { return IBAN; }
const std::vector<Card> &Cont::getCarduri() const { return carduri; }

bool Cont::retrageSuma(const double suma) {
    for (auto &card: carduri) {
        if (card.getSuma() >= suma) {
            card.scadeSuma(suma);
            return true;
        }
    }
    return false;
}

double Cont::getSoldTotal() const {
    double total = 0;
    for (const auto &c: carduri) total += c.getSuma();
    return total;
}

void Cont::adaugaSuma(const double suma) {
    if (!carduri.empty())
        carduri[0].adaugaSuma(suma);
}

const std::vector<Tranzactie> &Cont::getTranzactii() const { return tranzactii; }


bool Cont::adaugaCard(const Card &card) {
    if (static_cast<int>(carduri.size()) >= limitaCarduri)
        std::cout << "Eroare : limita de carduri a fost depasita!";
    else
        carduri.push_back(card);
    return true;
}

void Cont::adaugaTranzactie(const Tranzactie &t) {
    tranzactii.push_back(t);
}
