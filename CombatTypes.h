#pragma once
#include <string>

enum class ActionType
{
    Attack,
    UseItem,
    Block
};

struct ActionResult
{
    ActionType type;
    std::string actor;
    std::string target;

    std::string itemName;
    int damagePlanned = 0;
    int damageApplied = 0;
    int damageBlocked = 0;
    int healedPlanned = 0;
    bool isCritical = false;
    bool targetDied = false;
    bool usedFocus = false;
    bool cancelled = false;
};