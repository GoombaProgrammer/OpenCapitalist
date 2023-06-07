#pragma once

class Business {
public:
    std::string name;
    unsigned long long cost;
    unsigned long long income;
    int progress;
    int progressDefault;
    unsigned long long upgradeCost;
    int level = 1;

    Business(const std::string& name, unsigned long long cost, unsigned long long income, int progress)
        : name(name), cost(cost), income(income), progress(progress) {
        progressDefault = progress;
        upgradeCost = cost * 2;
    }
};
