#include "SecurityUtils.h"
#include <cctype>

SecurityUtils::SecurityLevel SecurityUtils::checkPasswordStrength(const std::string &pass) {
    if (pass.length() < 6) return SecurityLevel::WEAK;

    bool hasUpper = false;
    bool hasDigit = false;
    bool hasSpecial = false;

    for (char c: pass) {
        if (std::isupper(c)) hasUpper = true;
        else if (std::isdigit(c)) hasDigit = true;
        else if (std::ispunct(c)) hasSpecial = true;
    }

    if (hasUpper && hasDigit && hasSpecial && pass.length() >= 8)
        return SecurityLevel::STRONG;

    if ((hasUpper || hasDigit) && pass.length() >= 6)
        return SecurityLevel::MEDIUM;

    return SecurityLevel::WEAK;
}

std::string SecurityUtils::encryptDecrypt(const std::string &data) {
    // Cheie de criptare hardcodata (pe un singur char pt XOR simplu)
    std::string output = data;

    for (size_t i = 0; i < data.size(); i++) {
        char key = 'K';

        output[i] = data[i] ^ key;
    }
    return output;
}
