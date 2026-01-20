#ifndef OOP_CLIENT_H
#define OOP_CLIENT_H
#pragma once
#include <string>
#include <vector>
#include <iostream>
#include "Cont.h"

class Client {
    std::string nume;
    std::string prenume;
    std::string CNP;
    std::string parola;
    std::vector<Cont *> conturi;
    double venit;
    int scorCredit;

public:
    friend void swap(Client &first, Client &second) noexcept;
    explicit Client(std::string nume, std::string prenume, std::string CNP, std::string parola,
                    const double &venit, const int &scorCredit);

    Client(const Client &other);
    Client &operator=(const Client &other);
    ~Client();

    friend std::ostream &operator<<(std::ostream &os, const Client &c);

    [[nodiscard]] std::vector<Cont *> &getConturi();

    [[nodiscard]] [[maybe_unused]] std::string credit(double suma, int luni) const;

    [[nodiscard]] const std::string &getCNP() const;

    [[nodiscard]] const std::string &getNume() const;

    [[nodiscard]] [[maybe_unused]] const std::string &getPrenume() const;

    [[nodiscard]] double getVenit() const { return venit; }

    [[nodiscard]] int getScorCredit() const { return scorCredit; }

    [[nodiscard]] bool verificaParola(const std::string &p) const;

    [[maybe_unused]] void adaugaCont(Cont *c);
};
#endif