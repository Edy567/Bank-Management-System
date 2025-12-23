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

    void adaugaClient(const Client& c);
    void adaugaAngajat(const Angajat& a);

    Client* autentificareClient(const std::string& nume, const std::string& parola);

    void transfer(const std::string &ibanSursa, const std::string &ibanDestinatie, double suma, const std::string& moneda);


    static void schimbValutar(Client *client, double sumaSursa, const std::string &monedaSursa,
                              const std::string &monedaDestinatie);

    void platesteFactura(const std::string &ibanCont, double suma, const std::string &furnizor);

    const std::vector<Client> &getClienti() const { return clienti; }

    friend std::ostream &operator<<(std::ostream &os, const Banca &banca);
};

#endif