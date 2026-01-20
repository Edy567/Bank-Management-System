#ifndef CONFIG_H
#define CONFIG_H

class Config {
private:

    Config() = default;

    double comisionTransfer = 1.5; // 1.5 RON
    double limitaTransferSuspect = 10000.0; // 10.000 RON
    int maxIncercariLogin = 3;

public:

    Config(const Config&) = delete;


    static Config& getInstance() {
        static Config instance;
        return instance;
    }


    [[nodiscard]] [[maybe_unused]] double getComisionTransfer() const { return comisionTransfer; }
    [[nodiscard]] [[maybe_unused]] double getLimitaSuspecta() const { return limitaTransferSuspect; }
    [[nodiscard]] [[maybe_unused]] int getMaxLoginAttempts() const { return maxIncercariLogin; }

    [[maybe_unused]] void setComision(const double val) { comisionTransfer = val; }
};

#endif