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

Client* Banca::getClient(const std::string& cnp) {
    for(auto& c : clienti) {
        if(c.getCNP() == cnp) return &c;
    }
    return nullptr;
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

std::ostream &operator<<(std::ostream &os, const Banca &banca) {
    os << banca.nume << " \n";
    os << "Numar Clienti: " << banca.clienti.size() << "\n";
    for(const auto& c : banca.clienti) {
        os << c.getCNP() << "\n";
    }
    return os;
}