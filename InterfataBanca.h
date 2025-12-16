#ifndef OOP_INTERFATABANCA_H
#define OOP_INTERFATABANCA_H

#include <SFML/Graphics.hpp>
#include <string>
#include "Banca.h"

enum class AppState {
    LOGIN,
    DASHBOARD,
    TRANSFER,
    CREDIT_SIMULATOR
};

class UI_Banca {
private:
    Banca& banca;
    Client* clientLogat;
    sf::RenderWindow window;
    sf::Font font;
    AppState stareCurenta;

    std::string bufferNume;
    std::string bufferParola;
    int loginFocus;

    std::string inputBuffer;
    std::string mesajEroare;
    std::string infoMesaj;

    std::string transferIbanDest;
    std::string transferSuma;
    std::string transferMoneda;
    int focusIndex;


    void drawLogin();
    void drawDashboard();
    void drawTransfer();
    void drawCredit();

    void handleLoginInput(const sf::Event::TextEntered& textEvent);
    void handleTransferInput(const sf::Event::TextEntered& textEvent);

    void processClick(const sf::Vector2f &pos);

    static void centerText(sf::Text& text, float y);
    void incarcaDate(const std::string& path) const;
    void incarcaDateDeTest() const;

    sf::Color georgeBlue = sf::Color(20, 72, 158);
    sf::Color georgeCyan = sf::Color(0, 195, 255);
public:
    explicit UI_Banca(Banca& b);
    void run(const std::string& fisierDate);
};

#endif