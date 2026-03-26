#pragma once

#ifndef ADMINISTRATOR_H
#define ADMINISTRATOR_H

#include <string>

class Administrator {
public:
    void changePass();
private:
    std::string adminId;
    std::string adminPass;
};

#endif 