#include "Banca.h"
#include <utility>

Banca::Banca(std::string nume, std::vector<Client> clienti, std::vector<Angajat> angajati) : nume(std::move(nume)),
    clienti{std::move(clienti)}, angajati{std::move(angajati)} {
    std::cout << "(Banca) Banca " << this->nume << " a fost creata.\n";
}

void Banca::adaugaClient(const Client &c) {
    clienti.push_back(c);
}

void Banca::adaugaAngajat(const Angajat &a) {
    angajati.push_back(a);
}

Client *Banca::autentificareClient(const std::string &nume, const std::string &parola) {
    for (auto &c: clienti) {
        if (c.getNume() == nume && c.verificaParola(parola)) return &c;
    }
    return nullptr;
}

bool Banca::transfer(const std::string &ibanSursa, const std::string &ibanDestinatie, const int suma) {
    Cont *contSursa = nullptr;
    Cont *contDestinatie = nullptr;

    for (auto &client: clienti) {
        for (auto *cont: client.getConturi()) {
            if (cont->getIBAN() == ibanSursa)
                contSursa = cont;
            if (cont->getIBAN() == ibanDestinatie)
                contDestinatie = cont;
        }
    }

    if (!contSursa || !contDestinatie) {
        std::cout << "Eroare: Unul dintre IBAN-uri nu exista in banca.\n";
        return false;
    }

    try {
        if (contSursa->getSoldTotal() < suma) {
            std::cout << "Fonduri insuficiente.\n";
            return false;
        }

        if (contSursa->retrageSuma(suma)) {
            contDestinatie->adaugaSuma(suma);

            contSursa->adaugaTranzactie(Tranzactie(-suma, "Transfer catre " + ibanDestinatie));
            contDestinatie->adaugaTranzactie(Tranzactie(suma, "Transfer de la " + ibanSursa));

            std::cout << "Transfer reusit: " << suma << " RON.\n";
            return true;
        }
        return false;
    } catch (const std::exception &e) {
        std::cout << "Eroare tranzactie: " << e.what() << "\n";
        return false;
    }
}

std::ostream &operator<<(std::ostream &os, const Banca &banca) {
    os << banca.nume << " \n";
    os << "Numar Clienti: " << banca.clienti.size() << "\n";
    for (const auto &c: banca.clienti) {
        os << c.getCNP() << "\n";
    }
    return os;
}
