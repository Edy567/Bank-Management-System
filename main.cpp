#include <fstream>
#include <iostream>
#include <vector>
#include <string>


class Moneda {
    std::string cod;
    std::string nume;
    double cursValutar; // fata de RON

public:
    Moneda(const std::string &cod, const std::string &nume, const double &cursValutar) {
        this->cod = cod;
        this->nume = nume;
        this->cursValutar = cursValutar;
    }

    Moneda(const Moneda &other) = default;

    Moneda &operator=(const Moneda &other) = default;

    friend std::ostream &operator<<(std::ostream &os, const Moneda &moneda) {
        os << moneda.cod << " " << moneda.nume << " " << moneda.cursValutar;
        return os;
    }

    const std::string &getCod() const { return cod; }

    ~Moneda() = default;
};

class Card {
    double suma;
    std::string titular;
    std::string numarExp;
    std::string nrCard;
    Moneda moneda;

public:
    Card(const double &suma, const std::string &titular, const std::string &numarExp, const std::string &nrCard,
         const Moneda &moneda) : moneda(moneda) {
        this->suma = suma;
        this->titular = titular;
        this->numarExp = numarExp;
        this->nrCard = nrCard;
    }

    Card(const Card &other) = default;

    Card &operator=(const Card &other) = default;

    friend std::ostream &operator<<(std::ostream &os, const Card &card) {
        os << card.titular << " " << card.numarExp << " " << card.nrCard << " " << " \n " << card.moneda;;
        return os;
    }

    double getSuma() const { return suma; }
    const Moneda &getMoneda() const { return moneda; }

    ~Card() = default;
};

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

    Tranzactie(const Tranzactie &tr) = default;

    Tranzactie &operator=(const Tranzactie &tr) = default;

    friend std::ostream &operator<<(std::ostream &os, const Tranzactie &tr) {
        return os << tr.data << " " << tr.buyIBAN << " " << tr.sellIBAN;
    }

    ~Tranzactie() = default;
};


class Cont {

    std::string IBAN;
    std::vector<Card> carduri;
    std::vector<Tranzactie> tranzactii;

public:
    explicit Cont(const std::vector<Card> &carduri, const std::string &IBAN,
                  const std::vector<Tranzactie> &tranzactii) {
        this->carduri = carduri;
        this->IBAN = IBAN;
        this->tranzactii = tranzactii;
    }

    Cont(const Cont &other) = default;

    Cont &operator=(const Cont &other) = default;

    friend std::ostream &operator<<(std::ostream &os, const Cont &cont) {
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

    double getSumByCurr(const std::string &codMoneda) const {
        double total = 0.0;
        for (const auto &card: carduri) {
            if (card.getMoneda().getCod() == codMoneda) {
                // folosim codul monedei
                total += card.getSuma();
            }
        }
        return total;
    }

    ~Cont() = default;

    //[[nodiscard]] const std::string &getIBAN() const {
    //    return IBAN;
    //}
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
    [[nodiscard]] const std::vector<Cont>& getConturi() const {
        return conturi;
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
    fin >> std::ws;
    std::getline(fin, numeBanca);

    int nrClienti;
    fin >> nrClienti;

    std::vector<Client> clienti;

    for (int i = 0; i < nrClienti; ++i) {
        std::string nume, prenume, CNP;
        fin >> nume >> prenume >> CNP;

        int nrConturi;
        fin >> nrConturi;

        std::vector<Cont> conturi;

        for (int j = 0; j < nrConturi; ++j) {
            std::string IBAN;
            fin >> IBAN;

            int nrCarduri;
            fin >> nrCarduri;

            std::vector<Card> carduri;
            for (int k = 0; k < nrCarduri; ++k) {
                double suma;
                std::string titular, numarExp, nrCard, codMoneda, numeMoneda;
                double cursValutar;

                fin >> suma >> codMoneda >> numeMoneda >> cursValutar >> titular >> numarExp >> nrCard;
                Moneda moneda(codMoneda, numeMoneda, cursValutar);
                carduri.emplace_back(suma, titular, numarExp, nrCard, moneda);
            }

            int nrTranzactii;
            fin >> nrTranzactii;
            std::vector<Tranzactie> tranzactii;
            for (int k = 0; k < nrTranzactii; ++k) {
                int sumaTr;
                std::string data, buyIBAN, sellIBAN;
                fin >> sumaTr >> data >> buyIBAN >> sellIBAN;
                tranzactii.emplace_back(sumaTr, data, buyIBAN, sellIBAN);
            }

            conturi.emplace_back(carduri, IBAN, tranzactii);
        }

        clienti.emplace_back(nume, prenume, CNP, conturi);
    }

    Banca banca(numeBanca, clienti);


    std::cout << banca << std::endl;

    std::cout << "Total pe valuta\n";
    for (const auto &client : clienti) {
        for (const auto &cont : client.getConturi()) {
            double totalRON = cont.getSumByCurr("RON");
            double totalEUR = cont.getSumByCurr("EUR");

            std::cout << " Total RON: " << totalRON << "\n";
            std::cout << " Total EUR: " << totalEUR << "\n";
        }
    }

    fin.close();
    return 0;
}


