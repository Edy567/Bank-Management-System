#include "Banca.h"
#include "Exceptii.h"
#include <utility>

Banca::Banca(std::string numeBanca, std::vector<Client> clienti, std::vector<Angajat> angajati)
    : nume(std::move(numeBanca)) , clienti{std::move(clienti)}, angajati {std::move(angajati)}{
    std::cout << "(Banca) Banca " << this->nume << " a fost creata.\n";
}

void Banca::adaugaClient(const Client& c) {
    clienti.push_back(c);
}

void Banca::adaugaAngajat(const Angajat& a) {
    angajati.push_back(a);
}


Client* Banca::autentificareClient(const std::string& numeUtilizator, const std::string& parolaUtilizator) {
    for(auto& c : clienti) {
        if(c.getNume() == numeUtilizator && c.verificaParola(parolaUtilizator)) {
            return &c;
        }
    }
    return nullptr;
}

void Banca::transfer(const std::string &ibanSursa, const std::string &ibanDestinatie, double suma, const std::string& moneda) {
    Cont *contSursa = nullptr;
    Cont *contDestinatie = nullptr;

    for (auto &client : clienti) {
        for (auto *cont : client.getConturi()) {
            if (cont->getIBAN() == ibanSursa)
                contSursa = cont;
            if (cont->getIBAN() == ibanDestinatie)
                contDestinatie = cont;
        }
    }

    if (!contSursa) throw Eroare("Contul sursa nu a fost gasit!");
    if (!contDestinatie) throw Eroare("IBAN-ul destinatar nu exista in banca!");

    if (!contSursa->areCardInValuta(moneda)) {
        throw Eroare("Nu detineti niciun card in valuta " + moneda + " pe acest cont!");
    }

    if (!contDestinatie->areCardInValuta(moneda)) {
        throw Eroare("Destinatarul nu are un card atasat pentru valuta " + moneda + "!");
    }

    if (contSursa->getSoldValuta(moneda) < suma) {
        throw FonduriInsuficiente();
    }

    if (contSursa->retrageSuma(suma, moneda)) {
        contDestinatie->adaugaSuma(suma, moneda);

        contSursa->adaugaTranzactie(Tranzactie(-static_cast<int>(suma), "Transfer " + moneda + " catre " + ibanDestinatie));
        contDestinatie->adaugaTranzactie(Tranzactie(static_cast<int>(suma), "Transfer " + moneda + " de la " + ibanSursa));

        std::cout << "Transfer reusit: " << suma << " " << moneda << ".\n";
    } else {
        throw Eroare("Eroare interna la procesarea retragerii.");
    }
}

void Banca::schimbValutar(Client *client, double sumaSursa, const std::string &monedaSursa,
                          const std::string &monedaDestinatie) {
    Cont *contSursa = nullptr;
    Cont *contDestinatie = nullptr;


    for (auto *c: client->getConturi()) {
        if (c->areCardInValuta(monedaSursa)) {
            if (c->getSoldValuta(monedaSursa) >= sumaSursa) {
                contSursa = c;
                break;
            }
        }
    }


    for (auto *c: client->getConturi()) {
        if (c->areCardInValuta(monedaDestinatie)) {
            contDestinatie = c;
            break;
        }
    }

    if (!contSursa) throw Eroare("Fonduri insuficiente sau lipsa card in " + monedaSursa);
    if (!contDestinatie) throw Eroare("Nu detineti niciun card in " + monedaDestinatie + " pentru a primi banii.");

    auto getID = [](const std::string& m) -> int {
        if (m == "RON") return 1;
        if (m == "EUR") return 2;
        if (m == "USD") return 3;
        if (m == "GBP") return 4;
        return 0;
    };

    int idS = getID(monedaSursa);
    int idD = getID(monedaDestinatie);
    int cheie = idS * 10 + idD;
    double rata;

    switch (cheie) {
        case 12: rata = 0.20;
            break;
        case 21: rata = 4.97;
            break;
        case 13: rata = 0.21;
            break;
        case 31: rata = 4.60;
            break;
        case 14: rata = 0.17;
            break;
        case 41: rata = 5.80;
            break;
        case 23: rata = 1.08;
            break;
        case 32: rata = 0.92;
            break;
        default: rata = 1.0;
            break;
    }


    if (contSursa->retrageSuma(sumaSursa, monedaSursa)) {
        double sumaDestinatie = sumaSursa * rata;
        contDestinatie->adaugaSuma(sumaDestinatie, monedaDestinatie);

        contSursa->adaugaTranzactie(Tranzactie(-static_cast<int>(sumaSursa), "Schimb valutar OUT " + monedaSursa));
        contDestinatie->adaugaTranzactie(Tranzactie(static_cast<int>(sumaDestinatie),
                                                    "Schimb valutar IN " + monedaDestinatie));
    } else {
        throw Eroare("Eroare la procesarea schimbului.");
    }
}

void Banca::platesteFactura(const std::string &ibanCont, double suma, const std::string& furnizor) {
    Cont* cont = nullptr;
    for (auto &client: clienti) {
        for (auto *c: client.getConturi()) {
            if (c->getIBAN() == ibanCont) {
                cont = c;
                break;
            }
        }
    }
    if (!cont) throw Eroare("Cont invalid.");

    if (!cont->areCardInValuta("RON")) throw Eroare("Facturile se platesc doar din contul de RON.");
    if(cont->getSoldValuta("RON") < suma) throw FonduriInsuficiente();

    if(cont->retrageSuma(suma, "RON")) {
        cont->adaugaTranzactie(Tranzactie(-static_cast<int>(suma), "Plata factura: " + furnizor));
    }
}

std::ostream &operator<<(std::ostream &os, const Banca &banca) {
    os << banca.nume << " \n";
    os << "Numar Clienti: " << banca.clienti.size() << "\n";
    for(const auto& c : banca.clienti) {
        os << c.getCNP() << "\n";
    }
    return os;
}