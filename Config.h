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


    double getComisionTransfer() const { return comisionTransfer; }
    double getLimitaSuspecta() const { return limitaTransferSuspect; }
    int getMaxLoginAttempts() const { return maxIncercariLogin; }

    void setComision(double val) { comisionTransfer = val; }
};

#endif