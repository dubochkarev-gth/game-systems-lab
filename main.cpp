// --- main.cpp ---
#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>
#include <limits>
#include <unordered_map>
#include "Entity.h"
#include "CombatTypes.h"
#include "Inventory.h"
#include "ItemSystem.h"

using namespace std;

int randomInt(int min, int max)
{
    static mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

// clear screen helper
void clearScreen()
{
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
};

struct FighterStats
{
    std::string name;
    int damageDealt = 0;
    int damageBlocked = 0;
    int healingDone = 0;
    int remainingHP = 0;
};

enum class BattleResult
{
    PlayerWin,
    EnemyWin
};

struct BattleSummary
{
    BattleResult result;
    int rounds = 0;
    std::vector<FighterStats> fighters;
};

// STRUCTS

enum class TargetType
{
    Self,
    FirstAliveEnemy
};

struct PlannedAction
{
    Entity *actor = nullptr;
    ActionType type;
    TargetType targetType;
};

struct ResolvedAction
{
    Entity *actor = nullptr;
    ActionType type;
    Entity *target = nullptr;
};

struct BattleLog
{
    vector<ActionResult> actions;

    void clear()
    {
        actions.clear();
    }

    void add(const ActionResult &result)
    {
        actions.push_back(result);
    }
};

vector<Entity *> resolveTargets(
    const PlannedAction &action,
    const vector<Entity *> &entities);

bool validateAction(
    const PlannedAction &action,
    Entity *target,
    ActionResult &outResult);

void executeAction(
    const ResolvedAction &action,
    BattleLog &log);

void applyActionResult(
    ActionResult &result,
    Entity &target);

void startTurn(Entity &e);
void endTurn(Entity &e);

void renderBattleScreen(
    const vector<Entity *> &entities,
    const BattleLog &log,
    const vector<Entity *> &turnOrder);

void buildTurnOrder(
    const vector<Entity *> &entities,
    vector<Entity *> &turnOrder);

vector<PlannedAction> planTurn(
    const vector<Entity *> &turnOrder, bool interactive);

class Battle
{
private:
    vector<Entity *> entities;
    BattleLog log;
    vector<Entity *> turnOrder;
    unordered_map<std::string, FighterStats> statsMap;
    int roundCounter = 0;
    bool interactive = false;

public:
    Battle(std::vector<Entity *> ents, bool interactiveMode)
        : entities(ents), interactive(interactiveMode)
    {
    }

    BattleSummary run()
    {
        BattleSummary summary;
        roundCounter = 0;
        statsMap.clear();
        log.clear();
        turnOrder.clear();

        for (Entity *e : entities)
        {
            FighterStats fs;
            fs.name = e->get_name();
            statsMap[e->get_name()] = fs;
        }

        while (true)
        {

            roundCounter++;

            buildTurnOrder(entities, turnOrder);
            log.clear();

            std::vector<PlannedAction> plannedActions =
                planTurn(turnOrder, interactive);

            for (PlannedAction &action : plannedActions)
            {

                if (!action.actor || !action.actor->is_alive())
                    continue;

                startTurn(*action.actor);

                std::vector<Entity *> targets =
                    resolveTargets(action, entities);

                for (Entity *target : targets)
                {

                    size_t before = log.actions.size();

                    ResolvedAction resolved;
                    resolved.actor = action.actor;
                    resolved.type = action.type;
                    resolved.target = target;

                    ActionResult validationResult;

                    if (!validateAction(action, target, validationResult))
                    {
                        log.add(validationResult);
                        continue;
                    }

                    executeAction(resolved, log);

                    if (log.actions.size() > before)
                    {
                        ActionResult &last = log.actions.back();
                        applyActionResult(last, *target);

                        statsMap[last.actor].damageDealt += last.damageApplied;
                        statsMap[last.target].damageBlocked += last.damageBlocked;
                        statsMap[last.actor].healingDone += last.healedPlanned;

                        // --- Threat generation ---
                        if (last.damageApplied > 0)
                        {
                            float normalized =
                                static_cast<float>(last.damageApplied) /
                                static_cast<float>(target->get_max_hp());

                            action.actor->add_threat(normalized);
                        }

                        if (last.healedPlanned > 0)
                        {
                            float normalized =
                                static_cast<float>(last.healedPlanned) /
                                static_cast<float>(target->get_max_hp());

                            action.actor->add_threat(normalized * 0.5f);
                        }
                    }
                }

                endTurn(*action.actor);
            }

            // Threat decay after full round
            for (Entity *e : entities)
            {
                e->decay_threat(0.8f);
            }

            if (interactive)
                renderBattleScreen(entities, log, turnOrder);

            // Проверка победы
            bool playerAlive = false;
            bool enemiesAlive = false;

            for (Entity *e : entities)
            {

                if (e->getFaction() == Faction::Player && e->is_alive())
                    playerAlive = true;

                if (e->getFaction() == Faction::Enemy && e->is_alive())
                    enemiesAlive = true;
            }

            if (!playerAlive)
            {
                summary.result = BattleResult::EnemyWin;

                if (interactive)
                {
                    std::cout << "\n=== Battle Finished ===\n";
                    std::cout << "Enemies win!\n";
                }

                break;
            }

            if (!enemiesAlive)
            {
                summary.result = BattleResult::PlayerWin;

                if (interactive)
                {
                    std::cout << "\n=== Battle Finished ===\n";
                    std::cout << "Player wins!\n";
                }

                break;
            }

            if (interactive)
            {
                std::cout << "\nPress Enter to continue...";
                std::cin.ignore(numeric_limits<streamsize>::max(), '\n');
                std::cin.get();
            }
        }
        summary.rounds = roundCounter;

        for (Entity *e : entities)
        {
            statsMap[e->get_name()].remainingHP = e->get_hp();
        }

        for (auto &pair : statsMap)
        {
            summary.fighters.push_back(pair.second);
        }

        return summary;
    };
};

// SCREEN DRAW AFTER CALCULATION

void renderBattleScreen(
    const std::vector<Entity *> &entities,
    const BattleLog &log,
    const std::vector<Entity *> &turnOrder)
{
    clearScreen();

    std::cout << "====== BATTLE ======\n\n";

    for (const Entity *e : entities)
    {
        e->info();
    }

    std::cout << "\n--- Initiative order ---\n";

    for (const Entity *ent : turnOrder)
    {

        if (!ent->is_alive())
            continue;

        std::cout << ent->get_name() << " (" << ent->getInitiative() << ")"
                  << "-----";
    }

    std::cout << "\n";

    std::cout << "\n--- Last turn ---\n";

    for (const ActionResult &r : log.actions)
    {
        if (r.cancelled)
        {
            std::cout << r.actor << " tries to attack, but " << r.target
                      << " is already dead";
            continue;
        }
        if (r.type == ActionType::Attack)
        {
            std::cout << r.actor << " hits " << r.target
                      << " for " << r.damageApplied << ".";

            if (r.isCritical)
                std::cout << " (CRITICAL)";

            if (r.usedFocus)
                std::cout << " (FOCUSED)";
        }

        if (r.type == ActionType::Block)
        {
            std::cout << r.actor << " prepares to block";
        }

        if (r.damageBlocked > 0)
        {
            std::cout << " " << r.target << " blocks " << r.damageBlocked << " damage";
        }

        if (r.type == ActionType::UseItem && r.healedPlanned > 0)
        {
            std::cout << r.actor << " uses " << r.itemName << " and heals for "
                      << r.healedPlanned << " HP";
        }

        std::cout << std::endl;

        if (r.targetDied)
        {
            std::cout << r.target << " is defeated!" << std::endl;
        }
    }

    std::cout << "\n--------------------\n";
}

// Initiative calculator

void buildTurnOrder(const std::vector<Entity *> &entities,
                    std::vector<Entity *> &turnOrder)
{
    turnOrder = entities;

    sort(turnOrder.begin(), turnOrder.end(),
         [](Entity *a, Entity *b)
         {
             return a->getInitiative() > b->getInitiative();
         });
}

// Executor for battle

void executeAction(const ResolvedAction &action,
                   BattleLog &log)
{
    if (!action.actor || !action.actor->is_alive())
        return;

    ActionResult result;

    switch (action.type)
    {
    case ActionType::Attack:
        result = action.actor->attack(*action.target);
        break;

    case ActionType::Block:
        result = action.actor->block();
        break;

    case ActionType::UseItem:
        result = ItemSystem::useItem(*action.actor);
        break;
    }

    log.add(result);
};

std::vector<Entity *> resolveTargets(
    const PlannedAction &action,
    const std::vector<Entity *> &entities)
{
    std::vector<Entity *> result;

    if (action.targetType == TargetType::Self)
    {
        result.push_back(action.actor);
    }

    if (action.targetType == TargetType::FirstAliveEnemy)
    {
        Entity *best = nullptr;
        float maxThreat = -1.0f;

        for (Entity *e : entities)
        {
            if (e != action.actor &&
                e->is_alive() &&
                e->getFaction() != action.actor->getFaction())
            {
                if (e->get_threat() > maxThreat)
                {
                    maxThreat = e->get_threat();
                    best = e;
                }
            }
        }

        if (best)
            result.push_back(best);
    }

    return result;
}

bool validateAction(
    const PlannedAction &action,
    Entity *target,
    ActionResult &outResult)
{
    outResult.actor = action.actor ? action.actor->get_name() : "unknown";
    outResult.target = target ? target->get_name() : "none";
    outResult.type = action.type;
    outResult.cancelled = true;

    // 1. Actor must be alive
    if (!action.actor || !action.actor->is_alive())
    {
        return false;
    }

    // 2. Target required for attack
    if (action.type == ActionType::Attack && !target)
    {
        return false;
    }

    // 3. Target must be alive for attack
    if (action.type == ActionType::Attack && !target->is_alive())
    {
        return false;
    }

    // 4. Item availability
    if (action.type == ActionType::UseItem)
    {
        Inventory *inv = action.actor->getInventory();
        if (!inv || inv->empty())
            return false;
    }

    // OK
    outResult.cancelled = false;
    return true;
}

void applyActionResult(ActionResult &result,
                       Entity &target)
{
    switch (result.type)
    {
    case ActionType::Attack:
    {
        int actualDamage = target.receive_damage(result.damagePlanned);
        result.damageApplied = actualDamage;
        result.damageBlocked = result.damagePlanned - actualDamage;
        result.targetDied = !target.is_alive();
        break;
    }

    case ActionType::Block:
    {
        target.set_blocking(true);
        target.add_focus(1);
        break;
    }

    case ActionType::UseItem:
    {
        int before = target.get_hp();
        target.heal(result.healedPlanned);
        result.healedPlanned = target.get_hp() - before;
        break;
    }
    }
}

TargetType targetTypeSelection(ActionType a)
{
    if (a == ActionType::Attack)
        return TargetType::FirstAliveEnemy;
    else
        return TargetType::Self;
}

// Decision function

std::vector<PlannedAction> planTurn(
    const std::vector<Entity*>& turnOrder,
    bool interactive)
{
    std::vector<PlannedAction> plannedActions;

    for (Entity* actor : turnOrder)
    {
        if (!actor->is_alive())
            continue;

        PlannedAction action;
        action.actor = actor;

        if (interactive &&
            actor->getFaction() == Faction::Player)
        {
            int choice = 0;

            std::cout << "\n" << actor->get_name()
                      << " choose action:\n";
            std::cout << "1 - Attack\n";
            std::cout << "2 - Block\n";
            std::cout << "3 - Use Item\n";

            while (choice < 1 || choice > 3)
            {
                std::cin >> choice;
            }

            if (choice == 1)
                action.type = ActionType::Attack;
            else if (choice == 2)
                action.type = ActionType::Block;
            else
                action.type = ActionType::UseItem;
        }
        else
        {
            action.type = actor->decideAction();
        }

        action.targetType = targetTypeSelection(action.type);
        plannedActions.push_back(action);
    }

    return plannedActions;
}

void startTurn(Entity &e) {
    // future: status tick, regen, focus decay
};

void endTurn(Entity &e) {
    // future: cleanup, state transitions
};

// --------------------
// Main
// --------------------
int main()
{
    Player hero("Dark_Avanger", 100, 10, 5);
    Player hero2("Shadow_Blader", 90, 12, 4);

    Enemy kobold("Sneaky_Kody", 55, 15, 5, 3);
    Enemy orc("Gazkul_Trakka", 90, 9, 7, 4);
    Enemy kobold2("Ugly_Gobby", 55, 15, 5, 3);

    auto heroInv = std::make_unique<Inventory>();
    heroInv->add({"Small Potion", ItemType::Heal, 7});
    heroInv->add({"Small Potion", ItemType::Heal, 7});
    hero.attachInventory(std::move(heroInv));

    auto orcInv = std::make_unique<Inventory>();
    orcInv->add({"Crude Potion", ItemType::Heal, 10});
    orcInv->add({"Crude Potion", ItemType::Heal, 10});
    orc.attachInventory(std::move(orcInv));

    std::vector<Entity *> battleEntities = {&hero, &hero2, &kobold, &orc, &kobold2};

    hero.setAutoMode(true);
    hero2.setAutoMode(true);

    Battle battle(battleEntities, true);
    BattleSummary summary = battle.run();

    std::cout << "\n=== Battle Summary ===\n";

    std::cout << "Winner: ";
    if (summary.result == BattleResult::PlayerWin)
        std::cout << "Player\n";
    else
        std::cout << "Enemies\n";
    std::cout << "Rounds: " << summary.rounds << "\n";

    for (const auto &f : summary.fighters)
    {
        std::cout << f.name
                  << " | HP: " << f.remainingHP
                  << " | Dealt: " << f.damageDealt
                  << " | Blocked: " << f.damageBlocked
                  << " | Healed: " << f.healingDone
                  << std::endl;
    }

    return 0;
}