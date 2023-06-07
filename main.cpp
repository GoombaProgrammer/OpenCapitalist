// OpenCapitalist - A simple business tycoon game
// Copyright (C) 2023 GoombaProgrammer

// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.

// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.

// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.

#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <vector>
#include <string>
#include <iostream>

#include "player.h"
#include "save.h"

using namespace std;

const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

// Define colors
SDL_Color BLACK = { 0, 0, 0, 255 };
SDL_Color WHITE = { 255, 255, 255, 255 };


SDL_Window* window = nullptr;
SDL_Renderer* renderer = nullptr;
Player player;

// TTF font
TTF_Font* font = nullptr;

double calculateProgressBarWidth(double progressValue, double maxValue, double backgroundWidth) {
    double ratio = progressValue / maxValue;
    double progressBarWidth = ratio * backgroundWidth;
    return progressBarWidth;
}

void purchaseBusiness(int index) {
    Business business = player.lockedBusinesses[index];
    if (player.money >= business.cost) {
        player.money -= business.cost;
        player.lockedBusinesses.erase(player.lockedBusinesses.begin() + index);
        player.businesses.push_back(business);
    }
}

void updateProgress() {
    for (auto& business : player.businesses) {
        if (business.progress > 0)
            business.progress--;
        else {
            player.money += business.income;
            business.progress = business.progressDefault;
        }
    }
}

void renderText(const std::string& text, int x, int y, SDL_Color color) {
    SDL_Surface* surface = TTF_RenderText_Solid(font, text.c_str(), color);
    SDL_Texture* texture = SDL_CreateTextureFromSurface(renderer, surface);
    SDL_Rect rect = { x, y, surface->w, surface->h };
    SDL_RenderCopy(renderer, texture, nullptr, &rect);

    SDL_FreeSurface(surface);
    SDL_DestroyTexture(texture);
}

void renderProgressBar(int x, int y, int width, int height, int progress, int progressDefault) {
    SDL_Rect bgRect = { x, y, width, height };
    SDL_SetRenderDrawColor(renderer, 128, 128, 128, 255);
    SDL_RenderFillRect(renderer, &bgRect);

    // Calculate progress width so it fits in the background rect
    int progressWidth = calculateProgressBarWidth(progress, progressDefault, width);
    SDL_Rect progressRect = { x, y, progressWidth, height };
    SDL_SetRenderDrawColor(renderer, 0, 255, 0, 255);
    SDL_RenderFillRect(renderer, &progressRect);
}

void render() {
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 255);
    SDL_RenderClear(renderer);

    // Render player's money
    std::string moneyText = "Money: $" + std::to_string(player.money);
    renderText(moneyText, 10, 10, WHITE);

    // Render available businesses to purchase
    int yPos = 50;
    for (const auto& business : player.lockedBusinesses) {
        std::string businessText = business.name + " - Cost: $" + std::to_string(business.cost);
        renderText(businessText, 10, yPos, WHITE);
        yPos += 30;
    }

    // Render owned businesses with progress bars
    yPos = 200;
    for (const auto& business : player.businesses) {
        std::string businessText = business.name + " - Income: $" + std::to_string(business.income) + " - Upgrade Cost: $" + std::to_string(business.upgradeCost);
        renderText(businessText, 10, yPos, WHITE);
        renderProgressBar(10, yPos + 24, 200, 10, business.progress, business.progressDefault);
        yPos += 50;
    }

    SDL_RenderPresent(renderer);
}

int main() {
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();

    window = SDL_CreateWindow("OpenCapitalist - Business Tycoon", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED,
        SCREEN_WIDTH, SCREEN_HEIGHT, SDL_WINDOW_SHOWN);
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    // Load font
    font = TTF_OpenFont("arial.ttf", 24);
    if (!font) {
        SDL_Log("Failed to load font: %s", TTF_GetError());
        return 1;
    }

    // Create some initial businesses
    Business business1("Tux's Pizza", 10, 1, 100);
    Business business2("Tux's Burger", 50, 5, 300);
    Business business3("Tux's Car Wash", 500, 20, 1000);
    player.lockedBusinesses.push_back(business1);
    player.lockedBusinesses.push_back(business2);
    player.lockedBusinesses.push_back(business3);

    Save save;
    save.load();

    bool quit = false;
    SDL_Event e;

    SDL_AddTimer(10, [](Uint32 interval, void* param) -> Uint32 {
        updateProgress();
        param = param; // This skips the unused parameter warning
        return interval;
    }, nullptr);

    while (!quit) {
        while (SDL_PollEvent(&e)) {
            if (e.type == SDL_QUIT)
                quit = true;
            else if (e.type == SDL_MOUSEBUTTONDOWN) {
                // Handle mouse click event
                // Check if a business was clicked and purchase it
                int x, y;
                SDL_GetMouseState(&x, &y);
                if (y >= 50 && y <= 150) {
                    int index = (y - 50) / 30;
                    purchaseBusiness(index);
                }
                // Check if a unlocked business was clicked and upgrade it
                else if (y >= 200) {
                    int index = (y - 200) / 50;
                    Business& business = player.businesses[index];
                    if (player.money >= business.upgradeCost) {
                        player.money -= business.upgradeCost;
                        business.income *= 2;
                        business.upgradeCost *= 3;
                        business.level++;
                        if ((business.level % 5) == 0)
                            business.progressDefault /= 2;
                    }
                }
            }
        }
        render();
    }

    save.save();

    TTF_CloseFont(font);
    SDL_DestroyRenderer(renderer);
    SDL_DestroyWindow(window);
    TTF_Quit();
    SDL_Quit();

    return 0;
}
