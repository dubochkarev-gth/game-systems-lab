// --- Simulation.h ---
#pragma once

#include <string>
#include <unordered_map>

struct SimulationStats
{
    int runs = 0;

    int playerWins = 0;
    int enemyWins = 0;

    int totalRounds = 0;

    std::unordered_map<std::string, int> totalDamage;
    std::unordered_map<std::string, int> totalMaxHit;
};