// --- Invenotry.h ---
#pragma once
#include <vector>
#include <optional>
#include "Item.h"

class Inventory
{
private:
    std::vector<Item> items;

public:
    bool empty() const
    {
        return items.empty();
    }

    void add(const Item& item)
    {
        items.push_back(item);
    }

    std::optional<Item> takeFirst()
    {
        if (items.empty())
            return std::nullopt;

        Item item = items.front();
        items.erase(items.begin());
        return item;
    }
};