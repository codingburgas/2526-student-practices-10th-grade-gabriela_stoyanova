#pragma once
#ifndef USER_H
#define USER_H

#include <string>

class User {
public:
    bool hasAnyUsers();
    bool registerUser(const std::string& id, const std::string& pass);
    bool loginUser(const std::string& id, const std::string& pass);
    void userArea(const std::string& id);
};

#endif // USER_H
