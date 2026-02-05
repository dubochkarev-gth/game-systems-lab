#include <iostream>
#include <string>
#include <random>
#include <algorithm>
#include <vector>

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
};

struct PlannedAction {
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

public:
    Entity(string n, int h, int baseInitiative)
        : hp(h), max_hp(h), name(n) {
            stats.baseInitiative = baseInitiative;
        }

    int get_hp() const {
        return hp;
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

    virtual ActionType decideAction(Entity& target) {
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

public:
    Player(string name, int hp, int baseInitiative, int weapon)
        : Entity(name, hp, baseInitiative), weapon_bonus(weapon) {}

    int get_attack_power() const override {
        return 10 + weapon_bonus;
    }

    void info() const override {
        cout << get_name() << " HP: " << get_hp();
        
        if (get_focus() > 0)
        cout << " [Focus: " << get_focus() << "]";

        cout<< endl;
    }

    ActionType decideAction(Entity& target) override {
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

    return ActionType::Block; // safety net
}
};

// Decision making for Enemy

class EnemyAI{
    private:
    AIState state = AIState::Aggressive;

    public:
    void update(const Entity& self)
    {
        int hp = self.get_hp();

        if (hp < 20)
            state = AIState::Desperate;
        else if (hp < 40)
            state = AIState::Defensive;
        else
            state = AIState::Aggressive;
    }

    ActionType decideAction(const Entity& self) const 
    {
        switch (state) {
            case AIState::Aggressive:
                return ActionType::Attack;

            case AIState::Defensive:{
                int roll = randomInt(0, 1);
                if (roll == 0)
                    return ActionType::Attack;
                else if (self.hasItems())
                        return ActionType::UseItem;
                    return ActionType::Block;
            }

            case AIState::Desperate:
                return self.has_focus()
                    ? ActionType::Attack
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
    Enemy(string name, int hp,  int baseInitiative, int baseAtk, int str)
        : Entity(name, hp, baseInitiative),
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

    ActionType decideAction(Entity& target) override {
    ai.update(*this);
    return ai.decideAction(*this);
    }
};

//SCREEN DRAW AFTER CALCULATION

void renderBattleScreen(
    const Player& p,
    const Enemy& e,
    const BattleLog& log,
    const vector<Entity*>& turnOrder
) {
    clearScreen();

    cout << "====== BATTLE ======\n\n";

    p.info();
    e.info();

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

void executeAction(const PlannedAction& action,
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

Entity* findFirstAliveEnemy(
    Entity* actor,
    const vector<Entity*>& entities
) {
  for (Entity* e : entities) {
        if (e != actor && e->is_alive())
            return e;
    }
    return nullptr;
}

Entity* findEntityByName(
    const string& name,
    const vector<Entity*>& entities
){
    for (Entity* e : entities)
        if (e->get_name() == name)
            return e;
    return nullptr;
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

            

// Decision function

vector<PlannedAction> planTurn(
    const vector<Entity*>& turnOrder,
    const vector<Entity*>& entities
){
    vector<PlannedAction> plannedActions;

    for (Entity* actor : turnOrder) {

        if(!actor->is_alive())
        continue;
        

        Entity* target = findFirstAliveEnemy(actor, entities);

        if(!target)
        continue;

        PlannedAction action;
        action.actor = actor;
        action.type = actor->decideAction(*target);
        action.target = target;

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
void runBattle(Player& p, Enemy& e) {

    BattleLog log;

    vector<Entity*> entities = { &p, &e };
    vector<Entity*> turnOrder;

    while (true) {
    
    buildTurnOrder(entities, turnOrder);
    log.clear();
    vector<PlannedAction> plannedActions=
    planTurn(turnOrder, entities);
    
    for (PlannedAction& action : plannedActions){

        if (!action.actor || !action.actor->is_alive())
            continue;

        startTurn(*action.actor);

        size_t before = log.actions.size();

        executeAction(action, log);

        if (log.actions.size() > before) {
        ActionResult& last = log.actions.back();
        Entity* target = findEntityByName(last.target, entities);

        if (target)
            applyActionResult(last, *target);
        }

        endTurn(*action.actor);
    }
   
    renderBattleScreen(p, e, log, turnOrder);

    if (!p.is_alive()) {
        cout << "\n=== Battle Finished ===\n";
        cout << "Winner: " << e.get_name() << endl;
        break;
    }

    if (!e.is_alive()) {
        cout << "\n=== Battle Finished ===\n";
        cout << "Winner: " << p.get_name() << endl;
        break;
    }

    cout << "\nPress Enter to continue...";
    cin.ignore();
    cin.get();
    }
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

    runBattle(hero, orc);

    return 0;
}