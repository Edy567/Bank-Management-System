#include "InterfataBanca.h"
#include "Cont_Silver.h"
#include "Cont_Gold.h"
#include "Cont_premium.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

bool isHover(const sf::FloatRect& rect, const sf::Vector2f mousePos) {
    return rect.contains(mousePos);
}

UI_Banca::UI_Banca(Banca& b) : banca(b), clientLogat(nullptr), stareCurenta(AppState::LOGIN),  loginFocus(0), focusIndex(0) {
    window.create(sf::VideoMode({800, 900}), "George Banking App", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);

    if (!font.openFromFile("arial.ttf")) {
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "EROARE: Nu gasesc arial.ttf! Asigura-te ca fisierul exista.\n";
            exit(1);
        }
    }
}

void UI_Banca::incarcaDate(const std::string& path) const {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::cerr << "EROARE: Nu pot deschide fisierul: " << path << std::endl;
        return;
    }

    std::string buffer;

    while (fin >> buffer) {
        if (buffer == "ANGAJATI") {
            int nr;
            fin >> nr;
            for (int i = 0; i < nr; ++i) {
                std::string n;
                int id, s;
                fin >> n >> id >> s;
                std::ranges::replace(n, '_', ' ');
                banca.adaugaAngajat(Angajat(n, id, s));
            }
        }
        else if (buffer == "CLIENTI") {
            int nrClienti;
            fin >> nrClienti;
            std::cout << "--- DEBUG: Incarc " << nrClienti << " clienti ---" << std::endl;
            for (int i = 0; i < nrClienti; ++i) {
                std::string numeComplet, cnp, parola;
                double venit; int scor;

                fin >> numeComplet >> cnp >> parola >> venit >> scor;
                std::ranges::replace(numeComplet, '_', ' ');
                std::stringstream ss(numeComplet);
                std::string nume, prenume;
                ss >> nume >> prenume;

                Client temp(nume, prenume, cnp, parola, venit, scor);

                std::cout << "Client incarcat: " << nume << " " << prenume
                          << " | USER LOGIN: " << nume
                          << " | PAROLA: " << parola << std::endl;

                int nrConturi;
                fin >> nrConturi;
                for(int j = 0; j < nrConturi; ++j) {
                    std::string tipCont, iban;
                    int limita;
                    fin >> tipCont >> iban >> limita;
                    std::vector<Card> cards;
                    std::vector<Tranzactie> tr;

                    int nrCarduri;
                    fin >> nrCarduri;
                    for(int k = 0; k < nrCarduri; ++k) {
                        double s, curs;
                        std::string tit, exp, nr, mc, mn;
                        fin >> s >> tit >> exp >> nr >> mc >> mn >> curs;
                        std::ranges::replace(tit, '_', ' ');
                        cards.emplace_back(s, tit, exp, nr, Moneda(mc, mn, curs));
                    }

                    Cont* ptr = nullptr;
                    if (tipCont == "SILVER") ptr = new ContSilver(cards, iban, tr);
                    else if (tipCont == "GOLD") ptr = new ContGold(cards, iban, tr);
                    else if (tipCont == "PREMIUM") ptr = new ContPremium(cards, iban, tr);

                    if (ptr) temp.adaugaCont(ptr);
                }
                banca.adaugaClient(temp);
            }
        }
    }
    fin.close();
}

void UI_Banca::incarcaDateDeTest() const {
    std::cout << "test data" << std::endl;
    Client c1("Andrei", "Popa", "1950101", "parola1", 5000, 750);
    std::vector<Card> carduri1;
    carduri1.emplace_back(1000, "Visa Classic", "12/26", "40001234", Moneda("RON", "Leu", 1));
    std::vector<Tranzactie> tr1;
    Cont* cont1 = new ContSilver(carduri1, "RO10BTRL001", tr1);
    c1.adaugaCont(cont1);
    banca.adaugaClient(c1);

    Client c2("Elena", "Dumitrescu", "2990505", "parola2", 8500, 600);
    std::vector<Card> carduri2;
    carduri2.emplace_back(10000, "MasterCard Gold", "01/28", "50001234", Moneda("RON", "Leu", 1));
    std::vector<Tranzactie> tr2;
    Cont* cont2 = new ContGold(carduri2, "RO99BTRL002", tr2);
    c2.adaugaCont(cont2);
    std::vector<Card> carduri3;
    carduri3.emplace_back(50000, "Visa Infinite", "05/30", "60009999", Moneda("RON", "Leu", 1));
    std::vector<Tranzactie> tr3;
    Cont* cont3 = new ContPremium(carduri3, "RO55BTRL003", tr3);
    c2.adaugaCont(cont3);
    banca.adaugaClient(c2);
}

void UI_Banca::run(const std::string& fisierDate) {
    incarcaDate(fisierDate);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto* key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape && stareCurenta != AppState::LOGIN) {
                    stareCurenta = AppState::DASHBOARD;
                    inputBuffer.clear(); infoMesaj.clear();
                }
            }
            else if (const auto* txt = event->getIf<sf::Event::TextEntered>()) {
                if (stareCurenta == AppState::LOGIN) handleLoginInput(*txt);
                else if (stareCurenta == AppState::TRANSFER) handleTransferInput(*txt);
                else if (stareCurenta == AppState::CREDIT_SIMULATOR) {
                    if (txt->unicode == '\b') {
                        if (!inputBuffer.empty()) inputBuffer.pop_back();
                    }
                    else if ((txt->unicode >= '0' && txt->unicode <= '9') || txt->unicode == '.') {
                        inputBuffer += static_cast<char>(txt->unicode);
                    }
                }
            }
            else if (const auto* ms = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (ms->button == sf::Mouse::Button::Left)
                    processClick(sf::Vector2f(static_cast<float>(ms->position.x), static_cast<float>(ms->position.y)));
            }
        }
        window.clear(sf::Color(240, 240, 245));
        switch (stareCurenta) {
            case AppState::LOGIN: drawLogin(); break;
            case AppState::DASHBOARD: drawDashboard(); break;
            case AppState::TRANSFER: drawTransfer(); break;
            case AppState::CREDIT_SIMULATOR: drawCredit(); break;
        }
        window.display();
    }
}

void UI_Banca::drawLogin() {
    sf::RectangleShape header(sf::Vector2f(800.f, 200.f));
    header.setFillColor(georgeBlue);
    window.draw(header);
    sf::Text logo(font, "George", 50);
    logo.setFillColor(sf::Color::White);
    logo.setStyle(sf::Text::Bold);
    centerText(logo, 60.f);
    window.draw(logo);

    sf::Text lblNume(font, "Nume Utilizator:", 18);
    lblNume.setFillColor(sf::Color::Black);
    lblNume.setPosition(sf::Vector2f(250.f, 220.f));
    window.draw(lblNume);
    sf::RectangleShape boxNume(sf::Vector2f(300.f, 40.f));
    boxNume.setFillColor(sf::Color::White);
    boxNume.setOutlineColor(loginFocus == 0 ? georgeCyan : sf::Color::Black);
    boxNume.setOutlineThickness(2.f);
    boxNume.setPosition(sf::Vector2f(250.f, 245.f));
    window.draw(boxNume);

    sf::Text txtNume(font, bufferNume, 20);
    txtNume.setFillColor(sf::Color::Black);
    txtNume.setPosition(sf::Vector2f(255.f, 252.f));
    window.draw(txtNume);

    sf::Text lblPass(font, "Parola:", 18);
    lblPass.setFillColor(sf::Color::Black);
    lblPass.setPosition(sf::Vector2f(250.f, 300.f));
    window.draw(lblPass);

    sf::RectangleShape boxPass(sf::Vector2f(300.f, 40.f));
    boxPass.setFillColor(sf::Color::White);
    boxPass.setOutlineColor(loginFocus == 1 ? georgeCyan : sf::Color::Black);
    boxPass.setOutlineThickness(2.f);
    boxPass.setPosition(sf::Vector2f(250.f, 325.f));
    window.draw(boxPass);

    const std::string maskedPass(bufferParola.length(), '*');
    sf::Text txtPass(font, maskedPass, 20);
    txtPass.setFillColor(sf::Color::Black);
    txtPass.setPosition(sf::Vector2f(255.f, 332.f));
    window.draw(txtPass);

    sf::RectangleShape btn(sf::Vector2f(300.f, 50.f));
    btn.setFillColor(georgeCyan);
    btn.setPosition(sf::Vector2f(250.f, 380.f));
    window.draw(btn);

    sf::Text btnText(font, "Autentificare", 24);
    btnText.setFillColor(sf::Color::White);
    btnText.setPosition(sf::Vector2f(320.f, 390.f));
    window.draw(btnText);
    if (!mesajEroare.empty()) {
        sf::Text err(font, mesajEroare, 18);
        err.setFillColor(sf::Color::Red);
        centerText(err, 450.f);
        window.draw(err);
    }
}

void UI_Banca::drawDashboard() {
    if(!clientLogat) return;

    sf::RectangleShape header(sf::Vector2f(800.f, 80.f));
    header.setFillColor(georgeBlue);
    window.draw(header);

    std::string salutText = "Salut, ";
    salutText += clientLogat->getPrenume();
    sf::Text salut(font, salutText, 24);
    salut.setFillColor(sf::Color::White);
    salut.setPosition(sf::Vector2f(20.f, 25.f));
    window.draw(salut);

    sf::RectangleShape btnOut(sf::Vector2f(80.f, 30.f));
    btnOut.setPosition(sf::Vector2f(700.f, 25.f));
    btnOut.setFillColor(sf::Color(220, 50, 50));
    window.draw(btnOut);
    sf::Text txtOut(font, "Iesire", 16);
    txtOut.setPosition(sf::Vector2f(715.f, 30.f));
    window.draw(txtOut);

    float yStart = 100.f;
    auto& conturi = clientLogat->getConturi();

    if(conturi.empty()) {
        sf::Text gol(font, "Nu ai niciun cont activ.", 20);
        gol.setFillColor(sf::Color::Black);
        gol.setPosition(sf::Vector2f(50.f, 150.f));
        window.draw(gol);
    }

    for (auto* c : conturi) {
        std::string tipCont = "Standard";
        auto culoareCont = sf::Color(200, 200, 200);

        if (dynamic_cast<ContSilver*>(c)) {
            tipCont = "SILVER";
            culoareCont = sf::Color(192, 192, 192);
        } else if (dynamic_cast<ContGold*>(c)) {
            tipCont = "GOLD";
            culoareCont = sf::Color(255, 215, 0);
        } else if (dynamic_cast<ContPremium*>(c)) {
            tipCont = "PREMIUM";
            culoareCont = sf::Color(50, 50, 50);
        }

        size_t nrCarduri = c->getCarduri().size();
        float inaltimeBox = 70.f + (static_cast<float>(nrCarduri) * 30.f);
        if(inaltimeBox < 120.f) inaltimeBox = 120.f;

        sf::RectangleShape cardBg(sf::Vector2f(740.f, inaltimeBox));
        cardBg.setPosition(sf::Vector2f(30.f, yStart));
        cardBg.setFillColor(sf::Color::White);
        cardBg.setOutlineColor(culoareCont);
        cardBg.setOutlineThickness(3.f);
        window.draw(cardBg);

        sf::RectangleShape badge(sf::Vector2f(100.f, 25.f));
        badge.setPosition(sf::Vector2f(30.f, yStart));
        badge.setFillColor(culoareCont);
        window.draw(badge);

        sf::Text txtTip(font, tipCont, 14);
        txtTip.setFillColor(tipCont == "PREMIUM" ? sf::Color::White : sf::Color::Black);
        txtTip.setPosition(sf::Vector2f(40.f, yStart + 3.f));
        window.draw(txtTip);

        std::string ibanText = "IBAN: ";
        ibanText += c->getIBAN();
        sf::Text ibanTxt(font, ibanText, 20);
        ibanTxt.setFillColor(georgeBlue);
        ibanTxt.setPosition(sf::Vector2f(45.f, yStart + 35.f));
        window.draw(ibanTxt);

        float yCard = yStart + 65.f;

        for(const auto& card : c->getCarduri()) {
            std::string cardInfo = card.getTitular();
            cardInfo += " (";
            cardInfo += card.getNrCard().substr(0,4);
            cardInfo += "****)";

            sf::Text txtCardInfo(font, cardInfo, 16);
            txtCardInfo.setFillColor(sf::Color::Black);
            txtCardInfo.setPosition(sf::Vector2f(45.f, yCard));
            window.draw(txtCardInfo);

            std::string s = std::to_string(card.getSuma());
            s = s.substr(0, s.find('.') + 3);
            std::string valuta = card.getMoneda().getCod();

            std::string sumaText = s;
            sumaText += " ";
            sumaText += valuta;

            sf::Text txtSuma(font, sumaText, 16);
            txtSuma.setFillColor(sf::Color::Black);
            txtSuma.setStyle(sf::Text::Bold);
            txtSuma.setPosition(sf::Vector2f(550.f, yCard));
            window.draw(txtSuma);

            yCard += 30.f;
        }

        yStart += inaltimeBox + 20.f;
    }

    float btnY = 820.f;
    sf::RectangleShape btnTrans(sf::Vector2f(200.f, 50.f));
    btnTrans.setPosition(sf::Vector2f(50.f, btnY));
    btnTrans.setFillColor(georgeCyan);
    window.draw(btnTrans);

    sf::Text txtTrans(font, "Transfer", 20);
    txtTrans.setPosition(sf::Vector2f(100.f, btnY + 12.f));
    window.draw(txtTrans);

    sf::RectangleShape btnCredit(sf::Vector2f(200.f, 50.f));
    btnCredit.setPosition(sf::Vector2f(300.f, btnY));
    btnCredit.setFillColor(georgeBlue);
    window.draw(btnCredit);

    sf::Text txtCred(font, "Credit", 20);
    txtCred.setPosition(sf::Vector2f(360.f, btnY + 12.f));
    window.draw(txtCred);
}

void UI_Banca::drawTransfer() {
    sf::Text title(font, "Transfer Nou", 30);
    title.setFillColor(georgeBlue);
    title.setPosition(sf::Vector2f(50.f, 30.f));
    window.draw(title);
    sf::Text lblDest(font, "IBAN Destinatar:", 18);
    lblDest.setFillColor(sf::Color::Black);
    lblDest.setPosition(sf::Vector2f(50.f, 100.f));
    window.draw(lblDest);

    sf::RectangleShape boxDest(sf::Vector2f(400.f, 40.f));
    boxDest.setPosition(sf::Vector2f(50.f, 130.f));
    boxDest.setFillColor(sf::Color::White);
    boxDest.setOutlineColor(focusIndex == 0 ? georgeCyan : sf::Color::Black);
    boxDest.setOutlineThickness(focusIndex == 0 ? 3.f : 1.f);
    window.draw(boxDest);

    sf::Text txtDest(font, transferIbanDest, 20);
    txtDest.setFillColor(sf::Color::Black);
    txtDest.setPosition(sf::Vector2f(60.f, 138.f));
    window.draw(txtDest);

    sf::Text lblSuma(font, "Suma (RON):", 18);
    lblSuma.setFillColor(sf::Color::Black);
    lblSuma.setPosition(sf::Vector2f(50.f, 200.f));
    window.draw(lblSuma);

    sf::RectangleShape boxSuma(sf::Vector2f(200.f, 40.f));
    boxSuma.setPosition(sf::Vector2f(50.f, 230.f));
    boxSuma.setFillColor(sf::Color::White);
    boxSuma.setOutlineColor(focusIndex == 1 ? georgeCyan : sf::Color::Black);
    boxSuma.setOutlineThickness(focusIndex == 1 ? 3.f : 1.f);
    window.draw(boxSuma);

    sf::Text txtSuma(font, transferSuma, 20);
    txtSuma.setFillColor(sf::Color::Black);
    txtSuma.setPosition(sf::Vector2f(60.f, 238.f));
    window.draw(txtSuma);

    sf::RectangleShape btnSend(sf::Vector2f(200.f, 50.f));
    btnSend.setPosition(sf::Vector2f(50.f, 320.f));
    btnSend.setFillColor(georgeBlue);
    window.draw(btnSend);

    sf::Text txtSend(font, "TRIMITE", 20);
    txtSend.setStyle(sf::Text::Bold);
    txtSend.setPosition(sf::Vector2f(100.f, 332.f));
    window.draw(txtSend);
    sf::Text back(font, "ESC - Inapoi la Dashboard", 15);
    back.setFillColor(sf::Color(100, 100, 100));
    back.setPosition(sf::Vector2f(50.f, 750.f));
    window.draw(back);
    if(!infoMesaj.empty()) {
        sf::Text info(font, infoMesaj, 18);
        if(infoMesaj.find("Succes") != std::string::npos) info.setFillColor(sf::Color::Green);
        else info.setFillColor(sf::Color::Red);
        info.setPosition(sf::Vector2f(300.f, 330.f));
        window.draw(info);
    }
}

void UI_Banca::drawCredit() {
    sf::Text title(font, "Simulator Credit", 30);
    title.setFillColor(georgeBlue);
    title.setPosition(sf::Vector2f(50.f, 50.f));
    window.draw(title);
    sf::Text lbl(font, "Suma Dorita:", 20);
    lbl.setFillColor(sf::Color::Black);
    lbl.setPosition(sf::Vector2f(50.f, 150.f));
    window.draw(lbl);

    sf::RectangleShape box(sf::Vector2f(200.f, 40.f));
    box.setPosition(sf::Vector2f(200.f, 145.f));
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(georgeCyan);
    box.setOutlineThickness(2.f);
    window.draw(box);
    sf::Text inp(font, inputBuffer, 20);
    inp.setFillColor(sf::Color::Black);
    inp.setPosition(sf::Vector2f(210.f, 153.f));
    window.draw(inp);

    sf::RectangleShape btnCalc(sf::Vector2f(200.f, 50.f));
    btnCalc.setPosition(sf::Vector2f(50.f, 250.f));
    btnCalc.setFillColor(georgeCyan);
    window.draw(btnCalc);

    sf::Text txtCalc(font, "Calculeaza", 20);
    txtCalc.setFillColor(sf::Color::White);
    txtCalc.setPosition(sf::Vector2f(90.f, 262.f));
    window.draw(txtCalc);

    if(!infoMesaj.empty()) {
        sf::Text res(font, infoMesaj, 18);
        res.setFillColor(georgeBlue);
        res.setPosition(sf::Vector2f(50.f, 350.f));
        window.draw(res);
    }

    sf::Text back(font, "ESC - Inapoi", 15);
    back.setFillColor(sf::Color::Black);
    back.setPosition(sf::Vector2f(50.f, 750.f));
    window.draw(back);
}

void UI_Banca::handleLoginInput(const sf::Event::TextEntered& e) {
    std::string* target = (loginFocus == 0) ? &bufferNume : &bufferParola;
    if (e.unicode == '\t') {
        loginFocus = (loginFocus + 1) % 2;
    }
    else if (e.unicode == '\b') {
        if (!target->empty()) target->pop_back();
    }
    else if (e.unicode == '\r' || e.unicode == '\n') {
        processClick(sf::Vector2f(260.f, 390.f));
    }
    else if (e.unicode >= 32 && e.unicode < 128) {
        *target += static_cast<char>(e.unicode);
    }
}

void UI_Banca::handleTransferInput(const sf::Event::TextEntered& e) {
    std::string* t = (focusIndex == 0) ? &transferIbanDest : &transferSuma;
    if (e.unicode == '\b') {
        if (!t->empty()) t->pop_back();
    }
    else if (e.unicode == '\t') focusIndex = !focusIndex;
    else if (e.unicode >= 32 && e.unicode < 128) {
        *t += static_cast<char>(e.unicode);
    }
}

void UI_Banca::processClick(const sf::Vector2f &pos) {
    const float x = pos.x;
    const float y = pos.y;
    if (stareCurenta == AppState::LOGIN) {
        if (sf::FloatRect({250.f, 245.f}, {300.f, 40.f}).contains(pos)) loginFocus = 0;
        if (sf::FloatRect({250.f, 325.f}, {300.f, 40.f}).contains(pos)) loginFocus = 1;

        if (sf::FloatRect({250.f, 380.f}, {300.f, 50.f}).contains(pos)) {
            if (auto* c = banca.autentificareClient(bufferNume, bufferParola)) {
                clientLogat = c;
                stareCurenta = AppState::DASHBOARD;
                bufferNume.clear(); bufferParola.clear(); mesajEroare.clear();
            } else mesajEroare = "User sau Parola gresita!";
        }
    }
    else if (stareCurenta == AppState::DASHBOARD) {
        if (x >= 700 && x <= 780 && y >= 25 && y <= 55) { clientLogat = nullptr;
        stareCurenta = AppState::LOGIN; }

        if (x >= 50 && x <= 250 && y >= 820 && y <= 870) { stareCurenta = AppState::TRANSFER;
        infoMesaj = ""; }
        if (x >= 300 && x <= 500 && y >= 820 && y <= 870) { stareCurenta = AppState::CREDIT_SIMULATOR;
        inputBuffer = ""; infoMesaj = ""; }
    }
    else if (stareCurenta == AppState::TRANSFER) {
        if (sf::FloatRect({50.f, 130.f}, {400.f, 40.f}).contains(pos)) focusIndex = 0;
        if (sf::FloatRect({50.f, 230.f}, {200.f, 40.f}).contains(pos)) focusIndex = 1;

        if (x >= 50 && x <= 250 && y >= 320 && y <= 370) {
            if (clientLogat->getConturi().empty()) {
                infoMesaj = "Eroare: Nu ai conturi!";
                return;
            }

            std::string destinatarCurat = transferIbanDest;
            std::erase(destinatarCurat, ' ');
            std::cout << "--- DEBUG TRANSFER ---" << std::endl;
            std::cout << "IBAN UI (raw): '" << transferIbanDest << "'" << std::endl;
            std::cout << "IBAN Trimis la Banca: '" << destinatarCurat << "'" << std::endl;

            int sumaDeTrimis = 0;
            try { sumaDeTrimis = std::stoi(transferSuma); } catch(...) {
                infoMesaj = "Eroare: Suma invalida!";
                return;
            }

            bool transferReusit = false;
            bool fonduriGasite = false;

            for(const auto* contSursa : clientLogat->getConturi()) {
                if(contSursa->getSoldTotal() >= sumaDeTrimis) {
                    fonduriGasite = true;
                    std::cout << "Incerc transfer din contul: " << contSursa->getIBAN() << std::endl;
                    if(banca.transfer(contSursa->getIBAN(), destinatarCurat, sumaDeTrimis)) {
                        transferReusit = true;
                        break;
                    } else {
                        std::cout << "Banca a refuzat transferul."
                        << std::endl;
                    }
                }
            }

            if(transferReusit) {
                infoMesaj = "Succes! ";
                infoMesaj += std::to_string(sumaDeTrimis);
                infoMesaj += " RON trimisi.";
            } else {
                if (!fonduriGasite) infoMesaj = "Eroare: Fonduri insuficiente.";
                else infoMesaj = "Eroare: Destinatar invalid sau eroare sistem.";
            }
        }
    }
    else if (stareCurenta == AppState::CREDIT_SIMULATOR) {
        if (x >= 50 && x <= 250 && y >= 250 && y <= 300) {
            double v = 0;
            try { v = std::stod(inputBuffer); } catch(...) {}
            infoMesaj = clientLogat->credit(v, 12);
        }
    }
}

void UI_Banca::centerText(sf::Text& t, float y) {
    const auto r = t.getLocalBounds();
    t.setOrigin(r.position + r.size / 2.f);
    t.setPosition(sf::Vector2f(400.f, y));
}