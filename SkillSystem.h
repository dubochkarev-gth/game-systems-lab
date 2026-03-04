// ---SkillSystem.h ---
#pragma once
#include "Entity.h"
#include "CombatTypes.h"

class SkillSystem
{
public:
    static ActionResult burst(Entity& actor, Entity& target);
};