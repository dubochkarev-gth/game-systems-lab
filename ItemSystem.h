/// --- ItemSystem.h ---
#pragma once
#include "CombatTypes.h"
#include "Entity.h"
#include "Inventory.h"

class ItemSystem
{
public:
    static ActionResult useItem(Entity& actor);
};