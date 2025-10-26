#include <fstream>
#include <iostream>
#include <vector>


class Tranzactie {
    int suma;
    std::string data;
    std::string buyIBAN;
    std::string sellIBAN;

public:
    Tranzactie(const int &sum, const std::string &data, const std::string &buyIBAN, const std::string &sellIBAN) {
        suma = sum;
        this->data = data;
        this->buyIBAN = buyIBAN;
        this->sellIBAN = sellIBAN;
    }

    Tranzactie(const Tranzactie &tr) {
        suma = tr.suma;
        data = tr.data;
        buyIBAN = tr.buyIBAN;
        sellIBAN = tr.sellIBAN;
    }

    Tranzactie &operator=(const Tranzactie &tr) = default;

    friend std::ostream &operator<<(std::ostream &os, const Tranzactie &tr) {
        return os << tr.data << " " << tr.buyIBAN << " " << tr.sellIBAN;
    }

    ~Tranzactie() = default;
};


class Cont {
    int suma;
    std::string IBAN;
    std::vector<Tranzactie> tranzactii;

public:
    explicit Cont(const int &suma, const std::string &IBAN, const std::vector<Tranzactie> &tranzactii) {
        this->suma = suma;
        this->IBAN = IBAN;
        this->tranzactii = tranzactii;
    }

    Cont(const Cont &other) {
        suma = other.suma;
        IBAN = other.IBAN;
        tranzactii = other.tranzactii;
    }

    Cont &operator=(const Cont &other) = default;

    friend std::ostream &operator<<(std::ostream &os, const Cont &cont) {
        os << cont.suma << " " << cont.IBAN << " \n";
        for (const auto &tr: cont.tranzactii) {
            os << tr << " " << "\n";
        }
        return os;
    }

    ~Cont() = default;

    std::string getIBAN() {
        return IBAN;
    }
};

class Client {
    std::string nume;
    std::string prenume;
    std::string CNP;
    std::vector<Cont> conturi;

public:
    explicit Client(const std::string &nume, const std::string &prenume, const std::string &CNP,
                    const std::vector<Cont> &conturi) {
        this->nume = nume;
        this->prenume = prenume;
        this->CNP = CNP;
        this->conturi = conturi;
    }

    Client(const Client &other) {
        this->nume = other.nume;
        this->prenume = other.prenume;
        this->CNP = other.CNP;
        this->conturi = other.conturi;
    }

    Client &operator=(const Client &other) = default;

    friend std::ostream &operator<<(std::ostream &os, const Client &c) {
        os << c.nume << " " << c.prenume << " " << c.CNP << " ";
        os << "Conturi:\n";
        for (const auto &cont: c.conturi) {
            os << cont << "\n";
        }
        return os;
    }

    ~Client() = default;
};

class Banca {
    std::vector<Client> clienti;
    std::string nume;

public:
    explicit Banca(const std::string &nume, const std::vector<Client> &clienti) {
        this->nume = nume;
        this->clienti = clienti;
    }

    Banca(const Banca &other) {
        this->nume = other.nume;
        this->clienti = other.clienti;
    }

    Banca &operator=(const Banca &other) = default;

    friend std::ostream &operator<<(std::ostream &os, const Banca &banca) {
        os << banca.nume << " " << std::endl;
        for (const auto &cont: banca.clienti) {
            os << cont << "\n";
        }
        return os;
    }

    ~Banca() = default;
};


int main() {
    std::ifstream fin("tastatura.txt");
    if (!fin) {
        std::cerr << "Nu s-a putut deschide fisierul tastatura.txt\n";
        return 1;
    }

    std::string numeBanca;
    fin >> std::ws; // elimină spațiile inițiale
    std::getline(fin, numeBanca);

    int nrClienti;
    fin >> nrClienti;

    std::vector<Client> clienti;

    for (int i = 0; i < nrClienti; i++) {
        std::string nume, prenume, CNP;
        fin >> nume >> prenume >> CNP;

        int nrConturi;
        fin >> nrConturi;

        std::vector<Cont> conturi;

        for (int j = 0; j < nrConturi; j++) {
            int suma;
            std::string IBAN;
            fin >> suma >> IBAN;

            int nrTranzactii;
            fin >> nrTranzactii;

            std::vector<Tranzactie> tranzactii;

            for (int k = 0; k < nrTranzactii; k++) {
                int sumaTr;
                std::string data, buyIBAN, sellIBAN;
                fin >> sumaTr >> data >> buyIBAN >> sellIBAN;
                tranzactii.emplace_back(sumaTr, data, buyIBAN, sellIBAN);
            }

            conturi.emplace_back(suma, IBAN, tranzactii);
        }

        clienti.emplace_back(nume, prenume, CNP, conturi);
    }

    // Creăm banca cu datele citite
    Banca banca(numeBanca, clienti);

    // Afișăm tot ce am citit
    std::cout << banca << std::endl;

    fin.close();
    return 0;
}
