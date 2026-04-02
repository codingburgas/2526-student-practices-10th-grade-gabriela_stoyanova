#pragma once

#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <string>

class Administrator {
public:
    void changePass(const std::string& currentId = "");
    bool hasAnyAdmins();
    bool registerAdmin(const std::string& id, const std::string& pass);
    bool loginAdmin(const std::string& id, const std::string& pass);
    bool verifyCredentials(const std::string& id, const std::string& pass);
private:
    std::string adminId;
    std::string adminPass;
};

#endif 