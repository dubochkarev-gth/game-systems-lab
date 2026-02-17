#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>
#include <limits>
#include <unordered_map>

using namespace std;

//Forward Declaration;

class Entity;

//CONSTANTS

constexpr float FOCUS_BONUS_MULTIPLIER = 1.5f;
constexpr int CRIT_CHANCE_PERCENT = 20;
constexpr float CRIT_MULTIPLIER = 2.0f;
constexpr float BLOCK_BONUS_MULTIPLIER = 0.5f;

// --------------------
// Random helper
// --------------------
int randomInt(int min, int max) {
    static mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

// clear screen helper
void clearScreen() {
#ifdef _WIN32
    system("cls");
#else
    system("clear");
#endif
};

//ENUMS

enum class BattleResult {
    PlayerWin,
    EnemyWin
};

struct FighterStats {
    string name;
    int damageDealt = 0;
    int damageBlocked = 0;
    int healingDone = 0;
    int remainingHP = 0;
};

struct BattleSummary {
    BattleResult result;
    int rounds = 0;
    vector<FighterStats> fighters;
};

enum class Faction {
    Player,
    Enemy
};

enum class ActionType{
    Attack,
    UseItem,
    Block
};

enum class AIState {
    Aggressive,
    Defensive,
    Desperate
};

//STRUCTS

struct ActionResult {
    ActionType type;
    string actor;
    string target;

    string itemName;
    int damagePlanned = 0;
    int damageApplied = 0;
    int damageBlocked = 0;
    int healedPlanned = 0;
    bool isCritical = false;
    bool targetDied = false;
    bool usedFocus = false;
    bool cancelled = false;
};

enum class TargetType {
    Self,
    FirstAliveEnemy
};

struct PlannedAction {
    Entity* actor = nullptr;
    ActionType type;
    TargetType targetType;
};

struct ResolvedAction {
    Entity* actor = nullptr;
    ActionType type;
    Entity* target = nullptr;
};

struct BattleLog {
    vector<ActionResult> actions;

    void clear() {
        actions.clear();
    }

    void add(const ActionResult& result) {
        actions.push_back(result);
    }
};

struct Stats {
    int baseInitiative = 0;
    // future:
    // int speed;
    // int agility;
};

// Item class

enum class ItemType {
    Heal
};

struct Item {
    string name;
    ItemType type;
    int power;
};

// --------------------
// Base Entity class
// --------------------
class Entity {
protected:
    int hp;
    int max_hp;
    string name;
    bool isBlocking = false;
    int focus = 0;
    Stats stats;
    vector<Item> inventory;
    Faction faction;

public:
    Entity(string n, int h, int baseInitiative, Faction f)
        : hp(h), max_hp(h), name(n), faction(f) {
            stats.baseInitiative = baseInitiative;
        }

    int get_hp() const {
        return hp;
    }

    Faction getFaction() const {
        return faction;
    }
    
    void heal(int amount) {
        hp = min(hp + amount, max_hp);
    }

    string get_name() const {
        return name;
    }

    bool has_focus() const{
        return focus > 0;
        }

    void add_focus(int amount){
        focus += amount;
    }

    void set_blocking(bool block){
        isBlocking = block;
    }

    void consume_focus(){
        focus = 0;
    }

    int take_damage(int dmg) {
        int finalDamage = 0;
        
        if(isBlocking){
            finalDamage = dmg*BLOCK_BONUS_MULTIPLIER;
            isBlocking = false;
        }
        else{
            finalDamage = dmg;
        }
        hp -= finalDamage;
        if (hp < 0) {
            hp = 0;
        }

        return finalDamage;
    }
    
    bool is_alive() const {
        return hp > 0;
    }

    virtual int get_attack_power() const {
        return 10;
    }

    virtual void info() const = 0;

    ActionResult attack(Entity& target){
        ActionResult result;

        result.type = ActionType::Attack;
        result.actor = name;
        result.target = target.get_name();

        int dmg = randomInt(1, get_attack_power());

    // крит — пока простой
        bool crit = randomInt(1, 100) <= CRIT_CHANCE_PERCENT;
    if (crit) {
        dmg *= CRIT_MULTIPLIER;
        result.isCritical = true;
    }
    if (has_focus()) {
        dmg *= FOCUS_BONUS_MULTIPLIER;
        consume_focus();
        result.usedFocus = true;
    }
    result.damagePlanned = dmg;
    
    return result;
    }

    ActionResult block(){
        ActionResult result;
        result.type = ActionType::Block;
        result.actor = name;
        result.target = name;
        return result;
    }

    int calculateInitiative() const {
        return stats.baseInitiative;
    }

    int getInitiative() const {
        return calculateInitiative();
    }

    virtual ActionType decideAction() {
        return ActionType::Block;
    }

    int get_focus() const {
        return focus;
    }

    virtual bool hasItems() const {
        return !inventory.empty();
    }

    void addItem(const Item& item) {
        inventory.push_back(item);
    }

    virtual ActionResult useItem() {
        ActionResult result;
        result.type = ActionType::UseItem;
        result.actor = name;
        result.target = name;
        
        if (inventory.empty())
            return result;

        Item item = inventory.front();
        inventory.erase(inventory.begin());
        result.itemName = item.name;

        if (item.type == ItemType::Heal) {
            result.healedPlanned = item.power;
        }

        return result;
    }

};

// --------------------
// Player
// --------------------
class Player : public Entity {
protected:
    int weapon_bonus;
    bool autoMode = false;

public:
    Player(string name, int hp, int baseInitiative, int weapon)
    : Entity(name, hp, baseInitiative, Faction::Player),
      weapon_bonus(weapon) {}

    int get_attack_power() const override {
        return 10 + weapon_bonus;
    }

    void setAutoMode(bool value) {
        autoMode = value;
    }

    void info() const override {
        cout << get_name() << " HP: " << get_hp();
        
        if (get_focus() > 0)
        cout << " [Focus: " << get_focus() << "]";

        cout<< endl;
    }

    ActionType decideAction() override {

    if (autoMode) {
        // Простая логика автобоя

        if (get_hp() < 30 && hasItems())
            return ActionType::UseItem;

        if (has_focus())
            return ActionType::Attack;

        int roll = randomInt(1, 100);
        return (roll <= 70) ? ActionType::Attack : ActionType::Block;
    }

    // ===== interactive режим =====

    int playerChoice = 0;

    while (playerChoice < 1 || playerChoice > 3) {
        cout << "Player make a choice:\n";
        cout << "1 - Attack\n2 - Block\n3 - Use Item\n";
        cin >> playerChoice;
    }

    if (playerChoice == 1)
        return ActionType::Attack;

    if (playerChoice == 2)
        return ActionType::Block;

    if (playerChoice == 3) {
        if (!hasItems()) {
            cout << "No items left!\n";
            return ActionType::Block;
        }
        return ActionType::UseItem;
    }

    return ActionType::Block;
}
};

// Decision making for Enemy

class EnemyAI{
    private:
    AIState state = AIState::Aggressive;
    bool canHeal = false;
    bool hasFocus = false;


    public:
    void update(int hp, bool canHealNow, bool hasFocusNow)
    {
        canHeal = canHealNow;
        hasFocus = hasFocusNow;

        if (hp < 20)
            state = AIState::Desperate;
        else if (hp < 40)
            state = AIState::Defensive;
        else
            state = AIState::Aggressive;
    }

    ActionType decideAction() const 
    {
        switch (state) {
            case AIState::Aggressive:
                return ActionType::Attack;

            case AIState::Defensive:{
                int roll = randomInt(0, 1);
                if (roll == 0)
                    return ActionType::Attack;
                if (canHeal)
                    return ActionType::UseItem;
                return ActionType::Block;
            }

            case AIState::Desperate:
                return hasFocus ? ActionType::Attack
                                : ActionType::Block;
    }
    return ActionType::Attack;

    }
}; 

// --------------------
// Enemy
// --------------------
class Enemy : public Entity {
protected:
    int base_attack;
    int strength;
    EnemyAI ai;

public:
    Enemy(string name, int hp, int baseInitiative, int baseAtk, int str)
    : Entity(name, hp, baseInitiative, Faction::Enemy),
      base_attack(baseAtk),
      strength(str) {}

    int get_attack_power() const override {
        return base_attack + strength;
    }

    void info() const override {
        cout << get_name() << " HP: " << get_hp();
        
        if (get_focus() > 0)
            cout << " [Focus: " << get_focus() << "]";
        
        cout << endl;
    }

    ActionType decideAction() override {
     ai.update(
        get_hp(),
        hasItems(),
        has_focus()
    );
    return ai.decideAction();
    }
};

//SCREEN DRAW AFTER CALCULATION

void renderBattleScreen(
    const vector<Entity*>& entities,
    const BattleLog& log,
    const vector<Entity*>& turnOrder
) {
    clearScreen();

    cout << "====== BATTLE ======\n\n";

    for (const Entity* e : entities) {
        e->info();
    }

    cout << "\n--------------------\n";

    cout << "\n--- Initiative order ---\n\n";

    for (const Entity* ent : turnOrder){
        
        if (!ent->is_alive())
        continue;

        cout<< ent->get_name() << " (" << ent->getInitiative() << ")"
        << "-----";
        }
    
    cout << "\n";

    cout << "\n--- Last turn ---\n";

for (const ActionResult& r : log.actions) {
    if (r.cancelled){
        cout << r.actor << " tries to attack, but " << r.target 
        << " is already dead";
        continue;
    }
    if (r.type == ActionType::Attack) {
        cout << r.actor << " hits " << r.target
             << " for " << r.damageApplied << ".";

        if (r.isCritical)
            cout << " (CRITICAL)";

        if (r.usedFocus)
            cout << " (FOCUSED)";
    }

        if (r.type == ActionType::Block) {
            cout << r.actor << " prepares to block";
            }

        if (r.damageBlocked > 0) {
            cout << " " << r.target << " blocks " << r.damageBlocked << " damage";
            }

    if (r.type == ActionType::UseItem && r.healedPlanned > 0) {
    cout << r.actor << " uses " <<r.itemName << " and heals for "
         << r.healedPlanned << " HP";
    }

    cout << endl;

    if (r.targetDied) {
        cout << r.target << " is defeated!" << endl;
    }
}

    cout << "\n--------------------\n";
}

//Initiative calculator

void buildTurnOrder(const vector<Entity*>& entities,
                    vector<Entity*>& turnOrder) {
    turnOrder = entities;

    sort(turnOrder.begin(), turnOrder.end(),
        [](Entity* a, Entity* b) {
            return a->getInitiative() > b->getInitiative();
        });
}

//Executor for battle

void executeAction(const ResolvedAction& action,
                   BattleLog& log)
{
    if (!action.actor || !action.actor->is_alive())
        return;

    ActionResult result;

    switch (action.type) {
        case ActionType::Attack:
            result = action.actor->attack(*action.target);
            break;

        case ActionType::Block:
            result = action.actor->block();
            break;

        case ActionType::UseItem:
            result = action.actor->useItem();
            break;
    }

    log.add(result);
};

vector<Entity*> resolveTargets(
    const PlannedAction& action,
    const vector<Entity*>& entities
) {
    vector<Entity*> result;

    if (action.targetType == TargetType::Self) {
        result.push_back(action.actor);
    }

    if (action.targetType == TargetType::FirstAliveEnemy) {
        for (Entity* e : entities) {
            if (e != action.actor &&
                e->is_alive() &&
                e->getFaction() != action.actor->getFaction()) {
                result.push_back(e);
                break;
            }
        }
    }

    return result;
}

bool validateAction(
    const PlannedAction& action,
    Entity* target,
    ActionResult& outResult
) {
    outResult.actor = action.actor ? action.actor->get_name() : "unknown";
    outResult.target = target ? target->get_name() : "none";
    outResult.type = action.type;
    outResult.cancelled = true;

    // 1. Actor must be alive
    if (!action.actor || !action.actor->is_alive()) {
        return false;
    }

    // 2. Target required for attack
    if (action.type == ActionType::Attack && !target) {
        return false;
    }

    // 3. Target must be alive for attack
    if (action.type == ActionType::Attack && !target->is_alive()) {
        return false;
    }

    // 4. Item availability
    if (action.type == ActionType::UseItem && !action.actor->hasItems()) {
        return false;
    }

    // OK
    outResult.cancelled = false;
    return true;
}

void applyActionResult(ActionResult& result,
    Entity& target){
    switch (result.type) {
        case ActionType::Attack: {
            int actualDamage = target.take_damage(result.damagePlanned);
            result.damageApplied = actualDamage;
            result.damageBlocked = result.damagePlanned - actualDamage;
            result.targetDied = !target.is_alive();
            break;
            }

        case ActionType::Block: {
            target.set_blocking(true);
            target.add_focus(1);
            break;
            }

        case ActionType::UseItem: {
            int before = target.get_hp();
            target.heal(result.healedPlanned);
            result.healedPlanned = target.get_hp() - before;
            break;
            }       
        }
}

TargetType  targetTypeSelection(ActionType a){
        if (a == ActionType::Attack)
            return TargetType::FirstAliveEnemy;
        else
            return TargetType::Self;     
}            

// Decision function

vector<PlannedAction> planTurn(
    const vector<Entity*>& turnOrder
    ){
    vector<PlannedAction> plannedActions;

    for (Entity* actor : turnOrder) {

        if(!actor->is_alive())
        continue;
        
        PlannedAction action;
        action.actor = actor;
        action.type = actor->decideAction();
        action.targetType = targetTypeSelection(action.type);
        plannedActions.push_back(action);
    }
        
    return plannedActions;
};

void startTurn(Entity& e) {
    // future: status tick, regen, focus decay
};

void endTurn(Entity& e) {
    // future: cleanup, state transitions
};

// --------------------
// Battle
// --------------------
BattleSummary runBattle(vector<Entity*>& entities, bool interactive) {

    BattleSummary summary;
    unordered_map<string, FighterStats> statsMap;

    for (Entity* e : entities) {
        FighterStats fs;
        fs.name = e->get_name();
        statsMap[e->get_name()] = fs;
    }

    BattleLog log;
    vector<Entity*> turnOrder;

    int roundCounter = 0;

    while (true) {

        roundCounter++;

        buildTurnOrder(entities, turnOrder);
        log.clear();

        vector<PlannedAction> plannedActions =
            planTurn(turnOrder);

        for (PlannedAction& action : plannedActions) {

            if (!action.actor || !action.actor->is_alive())
                continue;

            startTurn(*action.actor);

            vector<Entity*> targets =
                resolveTargets(action, entities);

            for (Entity* target : targets) {

                size_t before = log.actions.size();

                ResolvedAction resolved;
                resolved.actor = action.actor;
                resolved.type = action.type;
                resolved.target = target;

                ActionResult validationResult;

                if (!validateAction(action, target, validationResult)) {
                    log.add(validationResult);
                    continue;
                }

                executeAction(resolved, log);

                if (log.actions.size() > before) {
                    ActionResult& last = log.actions.back();
                    applyActionResult(last, *target);
                    statsMap[last.actor].damageDealt += last.damageApplied;
                    statsMap[last.target].damageBlocked += last.damageBlocked;
                    statsMap[last.actor].healingDone += last.healedPlanned;
                }
            }

            endTurn(*action.actor);
        }
        if (interactive)
            renderBattleScreen(entities, log, turnOrder);

        // Проверка победы
        bool playerAlive = false;
        bool enemiesAlive = false;

        for (Entity* e : entities) {

            if (e->getFaction() == Faction::Player && e->is_alive())
                playerAlive = true;

            if (e->getFaction() == Faction::Enemy && e->is_alive())
                enemiesAlive = true;
            }

        if (!playerAlive) {
            summary.result = BattleResult::EnemyWin;

            if (interactive) {
                cout << "\n=== Battle Finished ===\n";
                cout << "Enemies win!\n";
            }

            break;
        }

        if (!enemiesAlive) {
        summary.result = BattleResult::PlayerWin;

            if (interactive) {
                cout << "\n=== Battle Finished ===\n";
                cout << "Player wins!\n";
            }

        break;
        }

        if (interactive) {
            cout << "\nPress Enter to continue...";
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cin.get();
        }

     
    }
    summary.rounds = roundCounter;

    for (Entity* e : entities) {
    statsMap[e->get_name()].remainingHP = e->get_hp();
}

    for (auto& pair : statsMap) {
        summary.fighters.push_back(pair.second);
    }

return summary;
}

// --------------------
// Main
// --------------------
int main() {
    Player hero("Dark_Avanger", 100, 10, 5);

    Enemy kobold("Sneaky_Kody", 50, 15, 5, 3);
    Enemy orc("Gazkul_Trakka", 90, 9, 7, 4);

    hero.addItem({ "Small Potion", ItemType::Heal, 7 });
    hero.addItem({ "Small Potion", ItemType::Heal, 7 });

    orc.addItem({ "Crude Potion", ItemType::Heal, 10 });
    orc.addItem({ "Crude Potion", ItemType::Heal, 10 });

    vector<Entity*> battleEntities = { &hero, &kobold, &orc };

    hero.setAutoMode(true);
    BattleSummary summary = runBattle(battleEntities, false);

    cout << "\n=== Battle Summary ===\n";

    cout << "Winner: ";
        if (summary.result == BattleResult::PlayerWin)
            cout << "Player\n";
        else
            cout << "Enemies\n";
            cout << "Rounds: " << summary.rounds << "\n";

    for (const auto& f : summary.fighters) {
        cout << f.name
            << " | HP: " << f.remainingHP
            << " | Dealt: " << f.damageDealt
            << " | Blocked: " << f.damageBlocked
            << " | Healed: " << f.healingDone
            << endl;
        }

    return 0;
}