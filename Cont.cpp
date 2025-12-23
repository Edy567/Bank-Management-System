#include "Cont.h"
#include <utility>
#include <algorithm>

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
//const std::vector<Tranzactie> &Cont::getTranzactii() const { return tranzactii; }


bool Cont::retrageSuma(const double suma, const std::string &moneda) {
    for (auto &card: carduri) {
        if (card.getMoneda().getCod() == moneda) {
            if (card.getSuma() >= suma) {
                card.scadeSuma(suma);
                return true;
            }
        }
    }
    return false;
}


void Cont::adaugaSuma(const double suma, const std::string &moneda) {
    bool adaugat = false;

    for (auto &card: carduri) {
        if (card.getMoneda().getCod() == moneda) {
            card.adaugaSuma(suma);
            adaugat = true;
            break;
        }
    }

    if (!adaugat && !carduri.empty()) {
        carduri[0].adaugaSuma(suma);
    }
}

//double Cont::getSoldTotal() const {
  //  double total = 0;
   // for (const auto &c: carduri) total += c.getSuma();
   // return total;
//}

double Cont::getSoldValuta(const std::string &moneda) const {
    for (const auto &c: carduri) {
        if (c.getMoneda().getCod() == moneda) return c.getSuma();
    }
    return 0.0;
}


bool Cont::areCardInValuta(const std::string &moneda) const {
    return std::ranges::any_of(carduri, [&](const auto &c) {
        return c.getMoneda().getCod() == moneda;
    });
}

//bool Cont::adaugaCard(const Card &card) {
    //if (static_cast<int>(carduri.size()) >= limitaCarduri) {
       // std::cout << "Eroare : limita de carduri a fost depasita!";
     //   return false;
   // }
    //carduri.push_back(card);
  //  return true;
//}

void Cont::adaugaTranzactie(const Tranzactie &t) {
    tranzactii.push_back(t);
}