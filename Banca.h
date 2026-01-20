#ifndef OOP_BANCA_H
#define OOP_BANCA_H

#pragma once
#include <vector>
#include <string>
#include <iostream>
#include "Client.h"
#include "Angajat.h"

class Banca {
    std::string nume;
    std::vector<Client> clienti;
    std::vector<Angajat> angajati;
public:
    Banca(std::string numeBanca, std::vector<Client> clienti, std::vector<Angajat> angajati);

    [[maybe_unused]] void adaugaClient(const Client &c);

    [[maybe_unused]] void adaugaAngajat(const Angajat &a);

    [[maybe_unused]] Client *autentificareClient(const std::string &nume, const std::string &parola);

    [[maybe_unused]] void transfer(const std::string &ibanSursa, const std::string &ibanDestinatie, double suma,
                                   const std::string &moneda);

    [[maybe_unused]] static void schimbValutar(Client *client, double sumaSursa, const std::string &monedaSursa,
                                               const std::string &monedaDestinatie);

    [[maybe_unused]] void platesteFactura(const std::string &ibanCont, double suma, const std::string &furnizor);

    [[nodiscard]] [[maybe_unused]] const std::vector<Client> &getClienti() const { return clienti; }
    [[nodiscard]] [[maybe_unused]] const std::vector<Angajat> &getAngajati() const { return angajati; }

    friend std::ostream &operator<<(std::ostream &os, const Banca &banca);
};
#endif