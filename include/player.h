#pragma once

#include "business.h"
#include <vector>


class Player {
public:
    unsigned long long money;
    std::vector<Business> businesses;
    std::vector<Business> lockedBusinesses;

    Player() : money(10) {}
};
