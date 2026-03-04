// ---SkillSystem.cpp ---

#include "SkillSystem.h"

extern int randomInt(int min, int max);

ActionResult SkillSystem::burst(Entity& actor, Entity& target)
{
    ActionResult result;

    result.type = ActionType::Burst;
    result.actor = actor.get_name();
    result.target = target.get_name();

    const int cost = 2;

    if (!actor.spend_momentum(cost))
    {
        result.cancelled = true;
        return result;
    }

    int dmg = randomInt(1, actor.get_attack_power());

    dmg = static_cast<int>(dmg * 3.0f);

    result.damagePlanned = dmg;

    return result;
}