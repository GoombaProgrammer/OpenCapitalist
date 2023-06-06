#pragma once

#include <fstream>
#include <string>
#include "player.h"

using namespace std;

extern Player player;

class Save {
public:
    static void save() {
        ofstream saveFile("save.bin");
        saveFile << player.money << endl;
        saveFile << player.businesses.size() << endl;
        for (Business& business : player.businesses) {
            saveFile << business.name << endl;
            saveFile << business.cost << endl;
            saveFile << business.income << endl;
            saveFile << business.progressDefault << endl;
        }
        saveFile << player.lockedBusinesses.size() << endl;
        for (Business& business : player.lockedBusinesses) {
            saveFile << business.name << endl;
            saveFile << business.cost << endl;
            saveFile << business.income << endl;
            saveFile << business.progressDefault << endl;
        }
        saveFile.close();
    }

    static void load() {
        ifstream saveFile("save.bin");
        // If save file doesn't exist, return
        if (!saveFile.good()) {
            saveFile.close();
            return;
        }
        player.businesses.clear();
        player.lockedBusinesses.clear();
        // For each line in save file
        string line;
        getline(saveFile, line);
        player.money = stoull(line);
        getline(saveFile, line);
        int businessCount = stoi(line);
        for (int i = 0; i < businessCount; i++) {
            getline(saveFile, line);
            string name = line;
            getline(saveFile, line);
            unsigned long long cost = stoull(line);
            getline(saveFile, line);
            unsigned long long income = stoull(line);
            getline(saveFile, line);
            int progress = stoi(line);
            Business business(name, cost, income, progress);
            player.businesses.push_back(business);
        }
        getline(saveFile, line);
        int lockedBusinessCount = stoi(line);
        for (int i = 0; i < lockedBusinessCount; i++) {
            getline(saveFile, line);
            string name = line;
            getline(saveFile, line);
            unsigned long long cost = stoull(line);
            getline(saveFile, line);
            unsigned long long income = stoull(line);
            getline(saveFile, line);
            int progress = stoi(line);
            Business business(name, cost, income, progress);
            player.lockedBusinesses.push_back(business);
        }
        saveFile.close();
    }
};
