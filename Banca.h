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
    Banca(std::string nume, std::vector<Client> clienti, std::vector<Angajat> angajati);

    void adaugaClient(const Client &c);

    void adaugaAngajat(const Angajat &a);

    Client *autentificareClient(const std::string &nume, const std::string &parola);

    bool transfer(const std::string &ibanSursa, const std::string &ibanDestinatie, int suma);

    friend std::ostream &operator<<(std::ostream &os, const Banca &banca);
};
#endif
