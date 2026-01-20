#ifndef SECURITY_UTILS_H
#define SECURITY_UTILS_H

#include <string>

class SecurityUtils {
public:
    enum class SecurityLevel { WEAK, MEDIUM, STRONG };

    static SecurityLevel checkPasswordStrength(const std::string& pass);
    

    static std::string encryptDecrypt(const std::string& data);
};

#endif