#include "InterfataBanca.h"
#include "Cont_Silver.h"
#include "Cont_Gold.h"
#include "Cont_premium.h"
#include "Exceptii.h"
#include <iostream>
#include <fstream>
#include <sstream>
#include <cmath>
#include <algorithm>

bool isHover(const sf::FloatRect &rect, const sf::Vector2f mousePos) {
    return rect.contains(mousePos);
}

UI_Banca::UI_Banca(Banca &b) : banca(b), clientLogat(nullptr), stareCurenta(AppState::LOGIN), loginFocus(0),
                               focusIndex(0) {
    window.create(sf::VideoMode({800, 900}), "Masonerie SRL", sf::Style::Titlebar | sf::Style::Close);
    window.setFramerateLimit(60);
    transferMoneda = "RON";
    exchangeMonedaSursa = "RON";
    exchangeMonedaDest = "EUR";
    billFurnizor = "Enel";

    if (!font.openFromFile("arial.ttf")) {
        if (!font.openFromFile("C:/Windows/Fonts/arial.ttf")) {
            std::cerr << "Eroare.\n";
            exit(1);
        }
    }
}

void UI_Banca::incarcaDate(const std::string &path) const {
    std::ifstream fin(path);
    if (!fin.is_open()) {
        std::cerr << "Eroare." << std::endl;
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
        } else if (buffer == "CLIENTI") {
            int nrClienti;
            fin >> nrClienti;
            std::cout << "DEBUG: Incarc " << nrClienti << " clienti " << std::endl;
            for (int i = 0; i < nrClienti; ++i) {
                std::string numeComplet, cnp, parola;
                double venit;
                int scor;

                fin >> numeComplet >> cnp >> parola >> venit >> scor;
                std::ranges::replace(numeComplet, '_', ' ');
                std::stringstream ss(numeComplet);
                std::string nume, prenume;
                ss >> nume >> prenume;

                Client temp(nume, prenume, cnp, parola, venit, scor);

                std::cout << "Client : " << nume << " " << prenume
                        << "  User: " << nume
                        << "  Parola: " << parola << std::endl;

                int nrConturi;
                fin >> nrConturi;
                for (int j = 0; j < nrConturi; ++j) {
                    std::string tipCont, iban;
                    int limita;
                    fin >> tipCont >> iban >> limita;
                    std::vector<Card> cards;
                    std::vector<Tranzactie> tr;

                    int nrCarduri;
                    fin >> nrCarduri;
                    for (int k = 0; k < nrCarduri; ++k) {
                        double s, curs;
                        std::string tit, exp, nr, mc, mn;
                        fin >> s >> tit >> exp >> nr >> mc >> mn >> curs;
                        std::ranges::replace(tit, '_', ' ');
                        cards.emplace_back(s, tit, exp, nr, Moneda(mc, mn, curs));
                    }

                    Cont *ptr = nullptr;
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



void UI_Banca::run(const std::string &fisierDate) {
    incarcaDate(fisierDate);

    while (window.isOpen()) {
        while (const std::optional event = window.pollEvent()) {
            if (event->is<sf::Event::Closed>()) window.close();
            else if (const auto *key = event->getIf<sf::Event::KeyPressed>()) {
                if (key->scancode == sf::Keyboard::Scancode::Escape && stareCurenta != AppState::LOGIN) {
                    stareCurenta = AppState::DASHBOARD;
                    inputBuffer.clear();
                    infoMesaj.clear();
                }
            } else if (const auto *txt = event->getIf<sf::Event::TextEntered>()) {
                if (stareCurenta == AppState::LOGIN) handleLoginInput(*txt);
                else if (stareCurenta == AppState::TRANSFER) handleTransferInput(*txt);
                else if (stareCurenta == AppState::EXCHANGE) handleExchangeInput(*txt);
                else if (stareCurenta == AppState::BILLS) handleBillsInput(*txt);
                else if (stareCurenta == AppState::CREDIT_SIMULATOR) {
                    if (txt->unicode == '\b') {
                        if (!inputBuffer.empty()) inputBuffer.pop_back();
                    } else if ((txt->unicode >= '0' && txt->unicode <= '9') || txt->unicode == '.') {
                        inputBuffer += static_cast<char>(txt->unicode);
                    }
                }
            } else if (const auto *ms = event->getIf<sf::Event::MouseButtonPressed>()) {
                if (ms->button == sf::Mouse::Button::Left)
                    processClick(sf::Vector2f(static_cast<float>(ms->position.x), static_cast<float>(ms->position.y)));
            }
        }
        window.clear(sf::Color(245, 247, 250));
        switch (stareCurenta) {
            case AppState::LOGIN: drawLogin(); break;
            case AppState::DASHBOARD: drawDashboard(); break;
            case AppState::TRANSFER: drawTransfer(); break;
            case AppState::CREDIT_SIMULATOR: drawCredit(); break;
            case AppState::EXCHANGE: drawExchange(); break;
            case AppState::BILLS: drawBills(); break;
            case AppState::ADMIN: drawAdmin(); break;
        }
        window.display();
    }
}

void UI_Banca::drawLogin() {
    sf::Text logo(font, "Masonerie Popesti-Leordeni", 50);

    logo.setFillColor(georgeBlue);
    logo.setStyle(sf::Text::Bold);
    centerText(logo, 150.f);
    window.draw(logo);

    sf::Text subtitle(font, "Smart Banking", 20);
    subtitle.setFillColor(sf::Color(100, 100, 100));
    centerText(subtitle, 210.f);
    window.draw(subtitle);

    sf::Text lblNume(font, "Utilizator", 16);
    lblNume.setFillColor(sf::Color(80, 80, 80));
    lblNume.setPosition(sf::Vector2f(220.f, 270.f));
    window.draw(lblNume);

    sf::RectangleShape boxNume(sf::Vector2f(360.f, 50.f));
    boxNume.setFillColor(sf::Color::White);
    boxNume.setOutlineColor(loginFocus == 0 ? georgeCyan : sf::Color(200, 200, 200));
    boxNume.setOutlineThickness(2.f);
    boxNume.setPosition(sf::Vector2f(220.f, 300.f));
    window.draw(boxNume);

    sf::Text txtNume(font, bufferNume, 20);
    txtNume.setFillColor(sf::Color::Black);
    txtNume.setPosition(sf::Vector2f(235.f, 312.f));
    window.draw(txtNume);

    sf::Text lblPass(font, "Parola", 16);
    lblPass.setFillColor(sf::Color(80, 80, 80));
    lblPass.setPosition(sf::Vector2f(220.f, 370.f));
    window.draw(lblPass);

    sf::RectangleShape boxPass(sf::Vector2f(360.f, 50.f));
    boxPass.setFillColor(sf::Color::White);
    boxPass.setOutlineColor(loginFocus == 1 ? georgeCyan : sf::Color(200, 200, 200));
    boxPass.setOutlineThickness(2.f);
    boxPass.setPosition(sf::Vector2f(220.f, 400.f));
    window.draw(boxPass);

    const std::string maskedPass(bufferParola.length(), '*');
    sf::Text txtPass(font, maskedPass, 20);
    txtPass.setFillColor(sf::Color::Black);
    txtPass.setPosition(sf::Vector2f(235.f, 412.f));
    window.draw(txtPass);

    sf::RectangleShape btn(sf::Vector2f(360.f, 60.f));
    btn.setFillColor(georgeBlue);
    btn.setPosition(sf::Vector2f(220.f, 500.f));
    window.draw(btn);

    sf::Text btnText(font, "AUTENTIFICARE", 20);
    btnText.setFillColor(sf::Color::White);
    btnText.setStyle(sf::Text::Bold);
    auto r = btnText.getLocalBounds();
    btnText.setOrigin(r.position + r.size / 2.f);
    btnText.setPosition(sf::Vector2f(400.f, 530.f));
    window.draw(btnText);

    if (!mesajEroare.empty()) {
        sf::Text err(font, mesajEroare, 16);
        err.setFillColor(sf::Color(220, 50, 50));
        centerText(err, 600.f);
        window.draw(err);
    }
}

void UI_Banca::drawDashboard() {
    if (!clientLogat) return;

    sf::RectangleShape header(sf::Vector2f(800.f, 100.f));
    header.setFillColor(georgeBlue);
    window.draw(header);

    std::string salutText = "Salut, ";
    salutText += clientLogat->getPrenume();
    sf::Text salut(font, salutText, 28);
    salut.setFillColor(sf::Color::White);
    salut.setStyle(sf::Text::Bold);
    salut.setPosition(sf::Vector2f(40.f, 35.f));
    window.draw(salut);

    sf::Text txtOut(font, "LOGOUT", 16);
    txtOut.setFillColor(sf::Color(255, 255, 255, 200));
    txtOut.setPosition(sf::Vector2f(700.f, 40.f));
    window.draw(txtOut);

    float yStart = 140.f;
    auto &conturi = clientLogat->getConturi();

    if (conturi.empty()) {
        sf::Text gol(font, "Nu ai niciun cont activ.", 20);
        gol.setFillColor(sf::Color::Black);
        gol.setPosition(sf::Vector2f(40.f, 150.f));
        window.draw(gol);
    }

    for (auto *c: conturi) {
        std::string tipCont = "Standard";
        auto culoareCont = sf::Color(200, 200, 200);

        if (dynamic_cast<ContSilver *>(c)) {
            tipCont = "SILVER";
            culoareCont = sf::Color(192, 192, 192);
        } else if (dynamic_cast<ContGold *>(c)) {
            tipCont = "GOLD";
            culoareCont = sf::Color(255, 215, 0);
        } else if (dynamic_cast<ContPremium *>(c)) {
            tipCont = "PREMIUM";
            culoareCont = sf::Color(50, 50, 50);
        }

        size_t nrCarduri = c->getCarduri().size();
        float inaltimeBox = 70.f + (static_cast<float>(nrCarduri) * 35.f);
        if (inaltimeBox < 130.f) inaltimeBox = 130.f;

        sf::RectangleShape cardBg(sf::Vector2f(720.f, inaltimeBox));
        cardBg.setPosition(sf::Vector2f(40.f, yStart));
        cardBg.setFillColor(sf::Color::White);
        cardBg.setOutlineColor(culoareCont);
        cardBg.setOutlineThickness(3.f);
        window.draw(cardBg);

        sf::RectangleShape badge(sf::Vector2f(100.f, 30.f));
        badge.setPosition(sf::Vector2f(40.f, yStart));
        badge.setFillColor(culoareCont);
        window.draw(badge);

        sf::Text txtTip(font, tipCont, 14);
        txtTip.setFillColor(tipCont == "PREMIUM" ? sf::Color::White : sf::Color::Black);
        txtTip.setStyle(sf::Text::Bold);
        txtTip.setPosition(sf::Vector2f(55.f, yStart + 5.f));
        window.draw(txtTip);

        std::string ibanText = c->getIBAN();
        sf::Text ibanTxt(font, ibanText, 22);
        ibanTxt.setFillColor(georgeBlue);
        ibanTxt.setStyle(sf::Text::Bold);
        ibanTxt.setPosition(sf::Vector2f(55.f, yStart + 45.f));
        window.draw(ibanTxt);

        float yCard = yStart + 80.f;

        for (const auto &card: c->getCarduri()) {
            std::string cardInfo = card.getTitular();
            cardInfo += "  ....";
            cardInfo += card.getNrCard().substr(card.getNrCard().length() > 4 ? card.getNrCard().length() - 4 : 0);

            sf::Text txtCardInfo(font, cardInfo, 18);
            txtCardInfo.setFillColor(sf::Color(60, 60, 60));
            txtCardInfo.setPosition(sf::Vector2f(55.f, yCard));
            window.draw(txtCardInfo);

            std::string s = std::to_string(card.getSuma());
            s = s.substr(0, s.find('.') + 3);
            std::string valuta = card.getMoneda().getCod();

            std::string sumaText = s + " " += valuta;

            sf::Text txtSuma(font, sumaText, 18);
            txtSuma.setFillColor(sf::Color::Black);
            txtSuma.setStyle(sf::Text::Bold);
            txtSuma.setPosition(sf::Vector2f(550.f, yCard));
            window.draw(txtSuma);

            yCard += 35.f;
        }

        yStart += inaltimeBox + 30.f;
    }

    float footerY = 780.f;

    sf::RectangleShape btnTrans(sf::Vector2f(160.f, 50.f));
    btnTrans.setPosition(sf::Vector2f(40.f, footerY));
    btnTrans.setFillColor(georgeCyan);
    window.draw(btnTrans);

    sf::Text txtTrans(font, "Transfer", 16);
    txtTrans.setStyle(sf::Text::Bold);
    txtTrans.setFillColor(sf::Color::White);
    txtTrans.setPosition(sf::Vector2f(80.f, footerY + 15.f));
    window.draw(txtTrans);

    sf::RectangleShape btnEx(sf::Vector2f(160.f, 50.f));
    btnEx.setPosition(sf::Vector2f(220.f, footerY));
    btnEx.setFillColor(georgeBlue);
    window.draw(btnEx);

    sf::Text txtEx(font, "Schimb", 16);
    txtEx.setStyle(sf::Text::Bold);
    txtEx.setFillColor(sf::Color::White);
    txtEx.setPosition(sf::Vector2f(270.f, footerY + 15.f));
    window.draw(txtEx);

    sf::RectangleShape btnBills(sf::Vector2f(160.f, 50.f));
    btnBills.setPosition(sf::Vector2f(400.f, footerY));
    btnBills.setFillColor(sf::Color(50, 50, 50));
    window.draw(btnBills);

    sf::Text txtBills(font, "Facturi", 16);
    txtBills.setStyle(sf::Text::Bold);
    txtBills.setFillColor(sf::Color::White);
    txtBills.setPosition(sf::Vector2f(450.f, footerY + 15.f));
    window.draw(txtBills);

    sf::RectangleShape btnCredit(sf::Vector2f(160.f, 50.f));
    btnCredit.setPosition(sf::Vector2f(580.f, footerY));
    btnCredit.setFillColor(georgeBlue);
    window.draw(btnCredit);

    sf::Text txtCred(font, "Credit", 16);
    txtCred.setStyle(sf::Text::Bold);
    txtCred.setFillColor(sf::Color::White);
    txtCred.setPosition(sf::Vector2f(630.f, footerY + 15.f));
    window.draw(txtCred);
}

void UI_Banca::drawTransfer() {
    sf::Text title(font, "Transfer", 32);
    title.setFillColor(georgeBlue);
    title.setStyle(sf::Text::Bold);
    centerText(title, 80.f);
    window.draw(title);

    sf::Text lblDest(font, "IBAN Destinatar", 16);
    lblDest.setFillColor(sf::Color(80, 80, 80));
    lblDest.setPosition(sf::Vector2f(200.f, 180.f));
    window.draw(lblDest);

    sf::RectangleShape boxDest(sf::Vector2f(400.f, 50.f));
    boxDest.setPosition(sf::Vector2f(200.f, 210.f));
    boxDest.setFillColor(sf::Color::White);
    boxDest.setOutlineColor(focusIndex == 0 ? georgeCyan : sf::Color(200, 200, 200));
    boxDest.setOutlineThickness(2.f);
    window.draw(boxDest);

    sf::Text txtDest(font, transferIbanDest, 20);
    txtDest.setFillColor(sf::Color::Black);
    txtDest.setPosition(sf::Vector2f(215.f, 222.f));
    window.draw(txtDest);

    sf::Text lblSuma(font, "Suma de transfer", 16);
    lblSuma.setFillColor(sf::Color(80, 80, 80));
    lblSuma.setPosition(sf::Vector2f(200.f, 300.f));
    window.draw(lblSuma);

    sf::RectangleShape boxSuma(sf::Vector2f(280.f, 50.f));
    boxSuma.setPosition(sf::Vector2f(200.f, 330.f));
    boxSuma.setFillColor(sf::Color::White);
    boxSuma.setOutlineColor(focusIndex == 1 ? georgeCyan : sf::Color(200, 200, 200));
    boxSuma.setOutlineThickness(2.f);
    window.draw(boxSuma);

    sf::Text txtSuma(font, transferSuma, 20);
    txtSuma.setFillColor(sf::Color::Black);
    txtSuma.setPosition(sf::Vector2f(215.f, 342.f));
    window.draw(txtSuma);

    sf::RectangleShape btnMoneda(sf::Vector2f(100.f, 50.f));
    btnMoneda.setPosition(sf::Vector2f(500.f, 330.f));
    btnMoneda.setFillColor(sf::Color(230, 230, 230));
    btnMoneda.setOutlineColor(georgeBlue);
    btnMoneda.setOutlineThickness(2.f);
    window.draw(btnMoneda);

    sf::Text txtMoneda(font, transferMoneda, 20);
    txtMoneda.setFillColor(georgeBlue);
    txtMoneda.setStyle(sf::Text::Bold);
    auto r = txtMoneda.getLocalBounds();
    txtMoneda.setOrigin(r.position + r.size / 2.f);
    txtMoneda.setPosition(sf::Vector2f(550.f, 355.f));
    window.draw(txtMoneda);

    sf::RectangleShape btnSend(sf::Vector2f(400.f, 60.f));
    btnSend.setPosition(sf::Vector2f(200.f, 450.f));
    btnSend.setFillColor(georgeBlue);
    window.draw(btnSend);

    sf::Text txtSend(font, "TRIMITE BANII", 20);
    txtSend.setStyle(sf::Text::Bold);
    txtSend.setFillColor(sf::Color::White);
    auto rs = txtSend.getLocalBounds();
    txtSend.setOrigin(rs.position + rs.size / 2.f);
    txtSend.setPosition(sf::Vector2f(400.f, 480.f));
    window.draw(txtSend);

    sf::Text back(font, "< Inapoi la Dashboard (ESC)", 16);
    back.setFillColor(sf::Color(150, 150, 150));
    centerText(back, 850.f);
    window.draw(back);

    if (!infoMesaj.empty()) {
        sf::Text info(font, infoMesaj, 18);
        if (infoMesaj.find("Succes") != std::string::npos) info.setFillColor(sf::Color(0, 150, 0));
        else info.setFillColor(sf::Color(220, 50, 50));
        centerText(info, 550.f);
        window.draw(info);
    }
}

void UI_Banca::drawExchange() {
    sf::Text title(font, "Schimb Valutar", 32);
    title.setFillColor(georgeBlue);
    title.setStyle(sf::Text::Bold);
    centerText(title, 80.f);
    window.draw(title);

    sf::Text lblSuma(font, "Suma de schimbat", 16);
    lblSuma.setFillColor(sf::Color(80, 80, 80));
    lblSuma.setPosition(sf::Vector2f(200.f, 180.f));
    window.draw(lblSuma);

    sf::RectangleShape boxSuma(sf::Vector2f(400.f, 50.f));
    boxSuma.setPosition(sf::Vector2f(200.f, 210.f));
    boxSuma.setFillColor(sf::Color::White);
    boxSuma.setOutlineColor(georgeCyan);
    boxSuma.setOutlineThickness(2.f);
    window.draw(boxSuma);

    sf::Text txtSuma(font, exchangeSuma, 20);
    txtSuma.setFillColor(sf::Color::Black);
    txtSuma.setPosition(sf::Vector2f(215.f, 222.f));
    window.draw(txtSuma);

    sf::RectangleShape btnFrom(sf::Vector2f(150.f, 50.f));
    btnFrom.setPosition(sf::Vector2f(200.f, 300.f));
    btnFrom.setFillColor(sf::Color(230, 230, 230));
    window.draw(btnFrom);

    sf::Text txtFrom(font, "DIN: " + exchangeMonedaSursa, 18);
    txtFrom.setFillColor(sf::Color::Black);
    txtFrom.setPosition(sf::Vector2f(215.f, 312.f));
    window.draw(txtFrom);

    sf::RectangleShape btnTo(sf::Vector2f(150.f, 50.f));
    btnTo.setPosition(sf::Vector2f(450.f, 300.f));
    btnTo.setFillColor(sf::Color(230, 230, 230));
    window.draw(btnTo);

    sf::Text txtTo(font, "IN: " + exchangeMonedaDest, 18);
    txtTo.setFillColor(sf::Color::Black);
    txtTo.setPosition(sf::Vector2f(465.f, 312.f));
    window.draw(txtTo);

    sf::RectangleShape btnSchimb(sf::Vector2f(400.f, 60.f));
    btnSchimb.setPosition(sf::Vector2f(200.f, 400.f));
    btnSchimb.setFillColor(georgeBlue);
    window.draw(btnSchimb);

    sf::Text txtSchimb(font, "EFECTUEAZA SCHIMB", 20);
    txtSchimb.setStyle(sf::Text::Bold);
    txtSchimb.setFillColor(sf::Color::White);
    auto rs = txtSchimb.getLocalBounds();
    txtSchimb.setOrigin(rs.position + rs.size / 2.f);
    txtSchimb.setPosition(sf::Vector2f(400.f, 430.f));
    window.draw(txtSchimb);

    sf::Text back(font, "< Inapoi (ESC)", 16);
    back.setFillColor(sf::Color(150, 150, 150));
    centerText(back, 850.f);
    window.draw(back);

    if (!infoMesaj.empty()) {
        sf::Text info(font, infoMesaj, 18);
        if (infoMesaj.find("Succes") != std::string::npos) info.setFillColor(sf::Color(0, 150, 0));
        else info.setFillColor(sf::Color(220, 50, 50));
        centerText(info, 550.f);
        window.draw(info);
    }
}

void UI_Banca::drawBills() {
    sf::Text title(font, "Plata Facturi", 32);
    title.setFillColor(georgeBlue);
    title.setStyle(sf::Text::Bold);
    centerText(title, 80.f);
    window.draw(title);

    sf::Text lblF(font, "Furnizor", 16);
    lblF.setFillColor(sf::Color(80, 80, 80));
    lblF.setPosition(sf::Vector2f(200.f, 180.f));
    window.draw(lblF);

    sf::RectangleShape btnF(sf::Vector2f(400.f, 50.f));
    btnF.setPosition(sf::Vector2f(200.f, 210.f));
    btnF.setFillColor(sf::Color(240, 240, 240));
    window.draw(btnF);

    sf::Text txtF(font, billFurnizor, 20);
    txtF.setFillColor(sf::Color::Black);
    txtF.setPosition(sf::Vector2f(215.f, 222.f));
    window.draw(txtF);

    sf::Text lblS(font, "Valoare Factura (RON)", 16);
    lblS.setFillColor(sf::Color(80, 80, 80));
    lblS.setPosition(sf::Vector2f(200.f, 300.f));
    window.draw(lblS);

    sf::RectangleShape boxS(sf::Vector2f(400.f, 50.f));
    boxS.setPosition(sf::Vector2f(200.f, 330.f));
    boxS.setFillColor(sf::Color::White);
    boxS.setOutlineColor(georgeCyan);
    boxS.setOutlineThickness(2.f);
    window.draw(boxS);

    sf::Text txtS(font, billSuma, 20);
    txtS.setFillColor(sf::Color::Black);
    txtS.setPosition(sf::Vector2f(215.f, 342.f));
    window.draw(txtS);

    sf::RectangleShape btnPay(sf::Vector2f(400.f, 60.f));
    btnPay.setPosition(sf::Vector2f(200.f, 450.f));
    btnPay.setFillColor(georgeBlue);
    window.draw(btnPay);

    sf::Text txtPay(font, "PLATESTE", 20);
    txtPay.setStyle(sf::Text::Bold);
    txtPay.setFillColor(sf::Color::White);
    auto rs = txtPay.getLocalBounds();
    txtPay.setOrigin(rs.position + rs.size / 2.f);
    txtPay.setPosition(sf::Vector2f(400.f, 480.f));
    window.draw(txtPay);

    sf::Text back(font, "< Inapoi (ESC)", 16);
    back.setFillColor(sf::Color(150, 150, 150));
    centerText(back, 850.f);
    window.draw(back);

    if (!infoMesaj.empty()) {
        sf::Text info(font, infoMesaj, 18);
        if (infoMesaj.find("Succes") != std::string::npos) info.setFillColor(sf::Color(0, 150, 0));
        else info.setFillColor(sf::Color(220, 50, 50));
        centerText(info, 550.f);
        window.draw(info);
    }
}

void UI_Banca::drawAdmin() {
    sf::Text title(font, "Admin Panel", 32);
    title.setFillColor(sf::Color(200, 50, 50));
    title.setStyle(sf::Text::Bold);
    centerText(title, 50.f);
    window.draw(title);

    float yPos = 120.f;
    const auto& clienti = banca.getClienti();

    for(const auto& c : clienti) {
        sf::RectangleShape row(sf::Vector2f(700.f, 40.f));
        row.setPosition(sf::Vector2f(50.f, yPos));
        row.setFillColor(sf::Color::White);
        window.draw(row);

        std::string info = c.getNume() + " " + c.getPrenume() + " | CNP: " + c.getCNP();
        sf::Text txt(font, info, 16);
        txt.setFillColor(sf::Color::Black);
        txt.setPosition(sf::Vector2f(60.f, yPos + 10.f));
        window.draw(txt);

        yPos += 50.f;
        if(yPos > 800) break;
    }

    sf::Text back(font, "Logout (ESC)", 16);
    back.setFillColor(sf::Color(100, 100, 100));
    centerText(back, 850.f);
    window.draw(back);
}


void UI_Banca::drawCredit() {
    sf::Text title(font, "Simulator Credit", 32);
    title.setFillColor(georgeBlue);
    title.setStyle(sf::Text::Bold);
    centerText(title, 80.f);
    window.draw(title);

    sf::Text lbl(font, "Suma Solicitata", 16);
    lbl.setFillColor(sf::Color(80, 80, 80));
    lbl.setPosition(sf::Vector2f(200.f, 200.f));
    window.draw(lbl);

    sf::RectangleShape box(sf::Vector2f(400.f, 50.f));
    box.setPosition(sf::Vector2f(200.f, 230.f));
    box.setFillColor(sf::Color::White);
    box.setOutlineColor(georgeCyan);
    box.setOutlineThickness(2.f);
    window.draw(box);

    sf::Text inp(font, inputBuffer, 20);
    inp.setFillColor(sf::Color::Black);
    inp.setPosition(sf::Vector2f(215.f, 242.f));
    window.draw(inp);

    sf::RectangleShape btnCalc(sf::Vector2f(400.f, 60.f));
    btnCalc.setPosition(sf::Vector2f(200.f, 350.f));
    btnCalc.setFillColor(georgeBlue);
    window.draw(btnCalc);

    sf::Text txtCalc(font, "CALCULEAZA ELIGIBILITATE", 20);
    txtCalc.setStyle(sf::Text::Bold);
    txtCalc.setFillColor(sf::Color::White);
    auto rc = txtCalc.getLocalBounds();
    txtCalc.setOrigin(rc.position + rc.size / 2.f);
    txtCalc.setPosition(sf::Vector2f(400.f, 380.f));
    window.draw(txtCalc);

    if (!infoMesaj.empty()) {
        sf::Text res(font, infoMesaj, 18);
        res.setFillColor(georgeBlue);
        centerText(res, 500.f);
        window.draw(res);
    }

    sf::Text back(font, "< Inapoi (ESC)", 16);
    back.setFillColor(sf::Color(150, 150, 150));
    centerText(back, 850.f);
    window.draw(back);
}

void UI_Banca::handleLoginInput(const sf::Event::TextEntered &e) {
    std::string *target = (loginFocus == 0) ? &bufferNume : &bufferParola;
    if (e.unicode == '\t') {
        loginFocus = (loginFocus + 1) % 2;
    } else if (e.unicode == '\b') {
        if (!target->empty()) target->pop_back();
    } else if (e.unicode == '\r' || e.unicode == '\n') {
        processClick(sf::Vector2f(400.f, 530.f));
    } else if (e.unicode >= 32 && e.unicode < 128) {
        *target += static_cast<char>(e.unicode);
    }
}

void UI_Banca::handleTransferInput(const sf::Event::TextEntered &e) {
    std::string *t = (focusIndex == 0) ? &transferIbanDest : &transferSuma;
    if (e.unicode == '\b') {
        if (!t->empty()) t->pop_back();
    } else if (e.unicode == '\t') focusIndex = !focusIndex;
    else if (e.unicode >= 32 && e.unicode < 128) {
        *t += static_cast<char>(e.unicode);
    }
}

void UI_Banca::handleExchangeInput(const sf::Event::TextEntered &e) {
    if (e.unicode == '\b') {
        if (!exchangeSuma.empty()) exchangeSuma.pop_back();
    } else if ( (e.unicode >= '0' && e.unicode <= '9') || e.unicode == '.') {
        exchangeSuma += static_cast<char>(e.unicode);
    }
}

void UI_Banca::handleBillsInput(const sf::Event::TextEntered &e) {
    if (e.unicode == '\b') {
        if (!billSuma.empty()) billSuma.pop_back();
    } else if ( (e.unicode >= '0' && e.unicode <= '9') || e.unicode == '.') {
        billSuma += static_cast<char>(e.unicode);
    }
}


void UI_Banca::processClick(const sf::Vector2f &pos) {
    const float x = pos.x;
    const float y = pos.y;
    if (stareCurenta == AppState::LOGIN) {
        if (sf::FloatRect({220.f, 300.f}, {360.f, 50.f}).contains(pos)) loginFocus = 0;
        if (sf::FloatRect({220.f, 400.f}, {360.f, 50.f}).contains(pos)) loginFocus = 1;

        if (sf::FloatRect({220.f, 500.f}, {360.f, 60.f}).contains(pos)) {
            if (bufferNume == "admin" && bufferParola == "admin") {
                stareCurenta = AppState::ADMIN;
                bufferNume.clear();
                bufferParola.clear();
                return;
            }

            if (auto *c = banca.autentificareClient(bufferNume, bufferParola)) {
                clientLogat = c;
                stareCurenta = AppState::DASHBOARD;
                bufferNume.clear();
                bufferParola.clear();
                mesajEroare.clear();
            } else mesajEroare = "User sau Parola gresita!";
        }
    } else if (stareCurenta == AppState::DASHBOARD) {
        if (x >= 680 && x <= 780 && y >= 30 && y <= 60) {
            clientLogat = nullptr;
            stareCurenta = AppState::LOGIN;
        }

        if (x >= 40 && x <= 200 && y >= 780 && y <= 830) {
            stareCurenta = AppState::TRANSFER;
            infoMesaj = "";
        }
        if (x >= 220 && x <= 380 && y >= 780 && y <= 830) {
            stareCurenta = AppState::EXCHANGE;
            infoMesaj = "";
            exchangeSuma = "";
        }
        if (x >= 400 && x <= 560 && y >= 780 && y <= 830) {
            stareCurenta = AppState::BILLS;
            infoMesaj = "";
            billSuma = "";
        }
        if (x >= 580 && x <= 740 && y >= 780 && y <= 830) {
            stareCurenta = AppState::CREDIT_SIMULATOR;
            inputBuffer = "";
            infoMesaj = "";
        }

    } else if (stareCurenta == AppState::TRANSFER) {
        if (sf::FloatRect({200.f, 210.f}, {400.f, 50.f}).contains(pos)) focusIndex = 0;
        if (sf::FloatRect({200.f, 330.f}, {280.f, 50.f}).contains(pos)) focusIndex = 1;

        if (sf::FloatRect({500.f, 330.f}, {100.f, 50.f}).contains(pos)) {
            if (transferMoneda == "RON") transferMoneda = "EUR";
            else if (transferMoneda == "EUR") transferMoneda = "GBP";
            else if (transferMoneda == "GBP") transferMoneda = "USD";
            else transferMoneda = "RON";
        }

        if (sf::FloatRect({200.f, 450.f}, {400.f, 60.f}).contains(pos)) {
            if (clientLogat->getConturi().empty()) {
                infoMesaj = "Eroare: Nu ai conturi!";
                return;
            }

            std::string destinatarCurat = transferIbanDest;
            std::erase(destinatarCurat, ' ');

            double sumaDeTrimis = 0;
            try {
                sumaDeTrimis = std::stod(transferSuma);
            } catch (...) {
                infoMesaj = "Eroare: Suma invalida!";
                return;
            }

            bool transferReusit = false;
            std::string ultimaEroare;

            for (const auto *contSursa: clientLogat->getConturi()) {
                try {
                    banca.transfer(contSursa->getIBAN(), destinatarCurat, sumaDeTrimis, transferMoneda);
                    transferReusit = true;
                    break;
                } catch (const Eroare &e) {
                    ultimaEroare = e.what();
                } catch (const std::exception &e) {
                    ultimaEroare = e.what();
                }
            }

            if (transferReusit) {
                infoMesaj = "Succes! ";
                infoMesaj += transferSuma;
                infoMesaj += " ";
                infoMesaj += transferMoneda;
                infoMesaj += " trimisi.";
            } else {
                if (!ultimaEroare.empty()) infoMesaj = ultimaEroare;
                else infoMesaj = "Eroare: Fonduri insuficiente sau cont invalid.";
            }
        }
    } else if (stareCurenta == AppState::EXCHANGE) {
        if(sf::FloatRect({200.f, 300.f}, {150.f, 50.f}).contains(pos)) {
            if (exchangeMonedaSursa == "RON") exchangeMonedaSursa = "EUR";
            else if (exchangeMonedaSursa == "EUR") exchangeMonedaSursa = "USD";
            else if (exchangeMonedaSursa == "USD") exchangeMonedaSursa = "GBP";
            else exchangeMonedaSursa = "RON";
        }
        if(sf::FloatRect({450.f, 300.f}, {150.f, 50.f}).contains(pos)) {
            if (exchangeMonedaDest == "RON") exchangeMonedaDest = "EUR";
            else if (exchangeMonedaDest == "EUR") exchangeMonedaDest = "USD";
            else if (exchangeMonedaDest == "USD") exchangeMonedaDest = "GBP";
            else exchangeMonedaDest = "RON";
        }

        if(sf::FloatRect({200.f, 400.f}, {400.f, 60.f}).contains(pos)) {
            double s = 0;
            try { s = std::stod(exchangeSuma); } catch(...) { infoMesaj = "Suma invalida"; return; }

            if(clientLogat->getConturi().empty()) return;
            std::string iban = clientLogat->getConturi()[0]->getIBAN();

            try {
                banca.schimbValutar(iban, s, exchangeMonedaSursa, exchangeMonedaDest);
                infoMesaj = "Schimb reusit!";
            } catch(const std::exception& e) {
                infoMesaj = e.what();
            }
        }

    } else if (stareCurenta == AppState::BILLS) {
        if(sf::FloatRect({200.f, 210.f}, {400.f, 50.f}).contains(pos)) {
            if(billFurnizor == "Enel") billFurnizor = "Digi";
            else if(billFurnizor == "Digi") billFurnizor = "Orange";
            else if(billFurnizor == "Orange") billFurnizor = "E.ON";
            else billFurnizor = "Enel";
        }
        if(sf::FloatRect({200.f, 450.f}, {400.f, 60.f}).contains(pos)) {
            double s = 0;
            try { s = std::stod(billSuma); } catch(...) { infoMesaj = "Suma invalida"; return; }

            if(clientLogat->getConturi().empty()) return;
            std::string iban = clientLogat->getConturi()[0]->getIBAN();

            try {
                banca.platesteFactura(iban, s, billFurnizor);
                infoMesaj = "Factura platita cu succes!";
            } catch(const std::exception& e) {
                infoMesaj = e.what();
            }
        }

    } else if (stareCurenta == AppState::ADMIN) {
        if (x >= 700) {
            stareCurenta = AppState::LOGIN;
        }
    } else if (stareCurenta == AppState::CREDIT_SIMULATOR) {
        if (sf::FloatRect({200.f, 350.f}, {400.f, 60.f}).contains(pos)) {
            double v = 0;
            try { v = std::stod(inputBuffer); } catch (...) {
            }
            infoMesaj = clientLogat->credit(v, 12);
        }
    }
}

void UI_Banca::centerText(sf::Text &t, float y) {
    const auto r = t.getLocalBounds();
    t.setOrigin(r.position + r.size / 2.f);
    t.setPosition(sf::Vector2f(400.f, y));
}