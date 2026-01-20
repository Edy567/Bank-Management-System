#ifndef SECURITY_UTILS_H
#define SECURITY_UTILS_H

#include <string>

class SecurityUtils {
public:
    enum class SecurityLevel { WEAK, MEDIUM, STRONG };

   [[maybe_unused]] static SecurityLevel checkPasswordStrength(const std::string &pass);


   [[maybe_unused]] static std::string encryptDecrypt(const std::string &data);
};

#endif
