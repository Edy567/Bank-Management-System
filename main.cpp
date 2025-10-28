#include <fstream>
#include <iostream>
#include <utility>
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

    [[nodiscard]] const std::string &getCod() const { return cod; }

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

    [[nodiscard]] double getSuma() const { return suma; }
    [[nodiscard]] const Moneda &getMoneda() const { return moneda; }
    void scadeSuma(double valoare) { suma -= valoare; }
    void adaugaSuma(double valoare) { suma += valoare; }

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

    [[nodiscard]] double getSumByCurr(const std::string &codMoneda) const {
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

    [[nodiscard]] const std::string &getIBAN() const {
        return IBAN;
    }
    bool retrageSuma(double suma) {
        for (auto &card : carduri) {
            if (card.getSuma() >= suma) {
                // aici presupunem că vrem să retragem de pe primul card suficient
                card.scadeSuma(suma);
                return true;
            }
        }
        return false; // fonduri insuficiente
    }

    void adaugaSuma(double suma) {
        if (!carduri.empty())
            carduri[0].adaugaSuma(suma);
    }

};

class Client {
    std::string nume;
    std::string prenume;
    std::string CNP;
    std::vector<Cont> conturi;
    double venit;
    int scorCredit;

public:
    explicit Client(const std::string &nume, const std::string &prenume, const std::string &CNP,
                    const std::vector<Cont> &conturi, const double &venit, const int &scorCredit) {
        this->nume = nume;
        this->prenume = prenume;
        this->CNP = CNP;
        this->conturi = conturi;
        this->venit = venit;
        this->scorCredit = scorCredit;
    }

    Client(const Client &other) {
        this->nume = other.nume;
        this->prenume = other.prenume;
        this->CNP = other.CNP;
        this->conturi = other.conturi;
        this->venit = other.venit;
        this->scorCredit = other.scorCredit;
    }

    Client &operator=(const Client &other) = default;

    friend std::ostream &operator<<(std::ostream &os, const Client &c) {
        os << c.nume << " " << c.prenume << " " << c.CNP << " " << c.venit << " " << c.scorCredit;
        os << "Conturi:\n";
        for (const auto &cont: c.conturi) {
            os << cont << "\n";
        }
        return os;
    }

    [[nodiscard]]  std::vector<Cont> &getConturi()  {
        return conturi;
    }

     [[nodiscard]] std::string credit(double suma, int luni) const {
        const double rata = (suma / luni) * 1.05;
        double raport = rata / venit;

        if (scorCredit >= 700 && raport < 0.3)
            return "Cerere aprobata.";
        else if (scorCredit >= 600 && raport < 0.4)
            return "Cererea trebuie analizata.";
        else
            return "Cerere respinsa";
    }

    [[nodiscard]] const std::string& getCNP() const {
        return CNP;
    }

    ~Client() = default;
};

class Angajat {
    std::string nume;
    int id;
    int salariu;

    public:
        Angajat(std::string nume, const int &id, const int &salariu) : nume(std::move(nume)), id(id), salariu(salariu) {};

        Angajat(const Angajat &other) = default;

        Angajat &operator=(const Angajat &other) = default;

        friend std::ostream &operator<<(std::ostream &os, const Angajat &ang) {
        os << ang.nume << " " << ang.id << " " << ang.salariu;
        return os;
    }
    ~Angajat() = default;
};



class Banca {
    std::vector<Client> clienti;
    std::vector<Angajat> angajati;
    std::string nume;

public:
    explicit Banca(const std::string &nume, const std::vector<Client> &clienti, const std::vector<Angajat> &angajati) {
        this->nume = nume;
        this->clienti = clienti;
        this->angajati = angajati;
    }

    Banca(const Banca &other) {
        this->nume = other.nume;
        this->clienti = other.clienti;
        this->angajati = other.angajati;
    }

    Banca &operator=(const Banca &other) = default;

    friend std::ostream &operator<<(std::ostream &os, const Banca &banca) {
        os << banca.nume << " " << std::endl;
        os<<" Clienti: ";
        for (const auto &cont: banca.clienti) {
            os << cont << "\n";
        }
        os << " Angajati: ";
        for (const auto &angajat: banca.angajati) {
            os << angajat << " ";
        }
        return os;
    }

    bool transfer(const std::string &ibanSursa, const std::string &ibanDestinatie, const double suma) {
         Cont *contSursa = nullptr;
         Cont *contDestinatie = nullptr;

        // Cautăm conturile
        for (auto &client : clienti) {
            for (auto &cont : client.getConturi()) {
                if (cont.getIBAN() == ibanSursa)
                    contSursa = &cont;
                if (cont.getIBAN() == ibanDestinatie)
                    contDestinatie = &cont;
            }
        }

        if (!contSursa || !contDestinatie) {
            std::cout <<  "Unul dintre IBAN-uri nu exista in banca.\n";
            return false;
        }

        // verificăm fondurile
        if (!contSursa->retrageSuma(suma)) {
            std::cout << "Fonduri insuficiente.\n";
            return false;
        }

        // efectuăm transferul
        contDestinatie->adaugaSuma(suma);

        std::cout << "Transfer reusit: " << suma << " RON trimis de la "
                  << ibanSursa << " la " << ibanDestinatie << "\n";
        return true;
    }




    ~Banca() = default;
};


int main() {
    std::string filename = "tastatura.txt";
    std::ifstream in(filename);

    std::string bancaNume = "Banca";
    std::vector<Client> clienti;
    std::vector<Angajat> angajati;

    if (!in.is_open()) {
        std::cout << "Fisierul '" << filename << "' nu a fost gasit.\n";
    } else {

        std::string line;

        if (!std::getline(in, bancaNume)) {
            std::cerr << "Fisier invalid: nu am putut citi numele bancii.\n";
            return 1;
        }

        // 2) numar clienti
        int numClienti = 0;
        if (!(in >> numClienti)) {
            std::cerr << "Fisier invalid: nu am putut citi numarul de clienti.\n";
            return 1;
        }
        std::getline(in, line); // consum rest linie

        for (int i = 0; i < numClienti; ++i) {
            // citim campurile clientului
            std::string nume, prenume, CNP;
            double venit = 0.0;
            int scorCredit = 0;

            if (!std::getline(in, nume)) { std::cerr << "Eroare citire nume client\n"; break; }
            if (!std::getline(in, prenume)) { std::cerr << "Eroare citire prenume client\n"; break; }
            if (!std::getline(in, CNP)) { std::cerr << "Eroare citire CNP client\n"; break; }

            if (!(in >> venit >> scorCredit)) { std::cerr << "Eroare citire venit/scor\n"; break; }
            std::getline(in, line); // consum rest linie

            // conturi
            int numConturi = 0;
            if (!(in >> numConturi)) { std::cerr << "Eroare citire numConturi\n"; break; }
            std::getline(in, line);

            std::vector<Cont> conturi_client;

            for (int c = 0; c < numConturi; ++c) {
                std::string IBAN;
                if (!std::getline(in, IBAN)) { std::cerr << "Eroare citire IBAN\n"; break; }

                int numCarduri = 0;
                if (!(in >> numCarduri)) { std::cerr << "Eroare citire numCarduri\n"; break; }
                std::getline(in, line);

                std::vector<Card> carduri;

                for (int k = 0; k < numCarduri; ++k) {
                    double suma = 0.0;
                    std::string titular, numarExp, nrCard;
                    std::string codMoneda, numeMoneda;
                    double cursValutar = 1.0;

                    if (!(in >> suma)) { std::cerr << "Eroare citire suma card\n"; break; }
                    std::getline(in, line); // consum rest linie

                    if (!std::getline(in, titular)) { std::cerr << "Eroare citire titular\n"; break; }
                    if (!std::getline(in, numarExp)) { std::cerr << "Eroare citire numarExp\n"; break; }
                    if (!std::getline(in, nrCard)) { std::cerr << "Eroare citire nrCard\n"; break; }

                    if (!std::getline(in, codMoneda)) { std::cerr << "Eroare citire codMoneda\n"; break; }
                    if (!std::getline(in, numeMoneda)) { std::cerr << "Eroare citire numeMoneda\n"; break; }

                    if (!(in >> cursValutar)) { std::cerr << "Eroare citire cursValutar\n"; break; }
                    std::getline(in, line);

                    Moneda m(codMoneda, numeMoneda, cursValutar);
                    Card card(suma, titular, numarExp, nrCard, m);
                    carduri.push_back(card);
                }

                int numTranzactii = 0;
                if (!(in >> numTranzactii)) { std::cerr << "Eroare citire numTranzactii\n"; break; }
                std::getline(in, line);

                std::vector<Tranzactie> tranzactii;
                for (int t = 0; t < numTranzactii; ++t) {
                    int sumaT = 0;
                    std::string data, buyIBAN, sellIBAN;
                    if (!(in >> sumaT)) { std::cerr << "Eroare citire suma tranzactie\n"; break; }
                    std::getline(in, line);
                    if (!std::getline(in, data)) { std::cerr << "Eroare citire data tranzactie\n"; break; }
                    if (!std::getline(in, buyIBAN)) { std::cerr << "Eroare citire buyIBAN\n"; break; }
                    if (!std::getline(in, sellIBAN)) { std::cerr << "Eroare citire sellIBAN\n"; break; }

                    Tranzactie tr(sumaT, data, buyIBAN, sellIBAN);
                    tranzactii.push_back(tr);
                } // tranzactii

                Cont cont(carduri, IBAN, tranzactii);
                conturi_client.push_back(cont);
            } // conturi

            Client client(nume, prenume, CNP, conturi_client, venit, scorCredit);
            clienti.push_back(client);
        } // clienti

        // citire angajati
        int numAngajati = 0;
        if (!(in >> numAngajati)) {
            // daca nu exista sectiune angajati, ramanem cu vector gol
        } else {
            std::getline(in, line);
            for (int a = 0; a < numAngajati; ++a) {
                std::string angNume;
                if (!std::getline(in, angNume)) { std::cerr << "Eroare citire nume angajat\n"; break; }
                int id = 0, salariu = 0;
                if (!(in >> id >> salariu)) { std::cerr << "Eroare citire id/salariu angajat\n"; break; }
                std::getline(in, line);
                Angajat ang(angNume, id, salariu);
                angajati.push_back(ang);
            }
        }

        in.close();
    }


    Banca banca(bancaNume, clienti, angajati);


    int opt = -1;
    while (opt != 0) {
        std::cout << "\n Meniu \n";
        std::cout << "1. Detalii banca \n";
        std::cout << "2. Transfer intre conturi \n";
        std::cout << "3. Cere un credit \n";
        std::cout << "0. Iesire\n";
        std::cout << "Optiunea: ";
        if (!(std::cin >> opt)) {
            std::cin.clear();
            std::cin.ignore(10000, '\n');
            opt = -1;
            continue;
        }

        if (opt == 1) {
            std::cout << banca << "\n";
        } else if (opt == 2) {
            std::string ibanS, ibanD;
            double suma;
            std::cout << "IBAN sursa: "; std::cin >> ibanS;
            std::cout << "IBAN destinatie: "; std::cin >> ibanD;
            std::cout << "Suma (RON): "; std::cin >> suma;
            banca.transfer(ibanS, ibanD, suma);
        } else if (opt == 3) {
            std::string cnp;
            double suma;
            int luni;
            std::cout << "Introdu CNP: ";
            std::cin >> cnp;
            std::cout << "Suma credit dorita: ";
            std::cin >> suma;
            std::cout << "Numar luni: ";
            std::cin >> luni;
            bool gasit = false;
            for (auto &cl : clienti) {
                if (cl.getCNP() == cnp) {
                    std::cout << "Rezultat: " << cl.credit(suma, luni) << "\n";
                    gasit = true;
                    break;
                }
            }
            if (!gasit) {
                std::cout << "Client cu CNP '" << cnp << "' nu a fost gasit in datele citite.\n";
            }

        }  else if (opt == 0) {
            std::cout << "Iesire...\n";
        } else {
            std::cout << "Optiune invalida.\n";
        }
    }

    return 0;
}


