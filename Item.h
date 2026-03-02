// --- Item.h ---
#pragma once
#include <string>

enum class ItemType
{
    Heal
};

struct Item
{
    std::string name;
    ItemType type;
    int power;
};