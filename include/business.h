#pragma once

class Business {
public:
    std::string name;
    unsigned long long cost;
    unsigned long long income;
    int progress;
    int progressDefault;

    Business(const std::string& name, unsigned long long cost, unsigned long long income, int progress)
        : name(name), cost(cost), income(income), progress(progress) {
        progressDefault = progress;
    }
};
