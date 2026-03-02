#include "Entity.h"
#include <algorithm>
#include <iostream>

extern int randomInt(int min, int max);

// =======================
// Entity
// =======================

Entity::Entity(std::string n, int h, int baseInitiative, Faction f)
    : hp(h), max_hp(h), name(n), faction(f)
{
    stats.baseInitiative = baseInitiative;
}

void Entity::info() const
{
    std::cout << name
              << " | HP: " << hp
              << " | Focus: " << focus
              << " | Initiative: " << stats.baseInitiative
              << std::endl;
}

int Entity::get_hp() const { return hp; }
int Entity::get_focus() const { return focus; }
std::string Entity::get_name() const { return name; }
Faction Entity::getFaction() const { return faction; }
bool Entity::is_alive() const { return hp > 0; }

void Entity::heal(int amount)
{
    hp = std::min(hp + amount, max_hp);
}

int Entity::receive_damage(int amount)
{
    if (isBlocking)
    {
        amount = static_cast<int>(amount * BLOCK_BONUS_MULTIPLIER);
        isBlocking = false;
    }

    hp -= amount;
    if (hp < 0)
        hp = 0;

    return amount;
}

void Entity::set_blocking(bool block)
{
    isBlocking = block;
}

void Entity::add_focus(int amount)
{
    focus += amount;
}

void Entity::consume_focus()
{
    focus = 0;
}

bool Entity::has_focus() const
{
    return focus > 0;
}

int Entity::get_attack_power() const
{
    return 10;
}

int Entity::getInitiative() const
{
    return stats.baseInitiative;
}

bool Entity::hasItems() const
{
    return !inventory.empty();
}

void Entity::addItem(const Item &item)
{
    inventory.push_back(item);
}


// =======================
// Combat logic
// =======================

ActionResult Entity::attack(Entity& target)
{
    ActionResult result;

    result.type = ActionType::Attack;
    result.actor = name;
    result.target = target.get_name();

    int dmg = randomInt(1, get_attack_power());

    bool crit = randomInt(1, 100) <= CRIT_CHANCE_PERCENT;
    if (crit)
    {
        dmg = static_cast<int>(dmg * CRIT_MULTIPLIER);
        result.isCritical = true;
    }

    if (has_focus())
    {
        dmg = static_cast<int>(dmg * FOCUS_BONUS_MULTIPLIER);
        consume_focus();
        result.usedFocus = true;
    }

    result.damagePlanned = dmg;
    return result;
}

ActionResult Entity::block()
{
    ActionResult result;
    result.type = ActionType::Block;
    result.actor = name;
    result.target = name;
    return result;
}

ActionResult Entity::useItem()
{
    ActionResult result;
    result.type = ActionType::UseItem;
    result.actor = name;
    result.target = name;

    if (inventory.empty())
        return result;

    Item item = inventory.front();
    inventory.erase(inventory.begin());

    result.itemName = item.name;

    if (item.type == ItemType::Heal)
        result.healedPlanned = item.power;

    return result;
}

ActionType Entity::decideAction()
{
    return ActionType::Block;
}


// =======================
// Player
// =======================

Player::Player(std::string name, int hp, int baseInitiative, int weapon)
    : Entity(name, hp, baseInitiative, Faction::Player),
      weapon_bonus(weapon)
{
}

int Player::get_attack_power() const
{
    return 10 + weapon_bonus;
}

void Player::setAutoMode(bool value)
{
    autoMode = value;
}

void Player::info() const
{
    std::cout << get_name()
              << " HP: " << get_hp();

    if (get_focus() > 0)
        std::cout << " [Focus: " << get_focus() << "]";

    std::cout << std::endl;
}

ActionType Player::decideAction()
{
    if (autoMode)
    {
        if (get_hp() < max_hp*50/100 && hasItems())
            return ActionType::UseItem;

        if (has_focus())
            return ActionType::Attack;

        int roll = randomInt(1, 100);
        return (roll <= 40) ? ActionType::Attack : ActionType::Block;
    }

    int playerChoice = 0;

    while (playerChoice < 1 || playerChoice > 3)
    {
        std::cout << "Player make a choice:\n";
        std::cout << "1 - Attack\n2 - Block\n3 - Use Item\n";
        std::cin >> playerChoice;
    }

    if (playerChoice == 1)
        return ActionType::Attack;

    if (playerChoice == 2)
        return ActionType::Block;

    if (playerChoice == 3)
    {
        if (!hasItems())
        {
            std::cout << "No items left!\n";
            return ActionType::Block;
        }
        return ActionType::UseItem;
    }

    return ActionType::Block;
}

// =======================
// Enemy
// =======================

Enemy::Enemy(std::string name, int hp, int baseInitiative, int baseAtk, int str)
    : Entity(name, hp, baseInitiative, Faction::Enemy),
      base_attack(baseAtk),
      strength(str)
{
}

int Enemy::get_attack_power() const
{
    return base_attack + strength;
}

void EnemyAI::update(int hp, bool canHealNow, bool hasFocusNow)
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

ActionType EnemyAI::decideAction() const
{
    switch (state)
    {
        case AIState::Aggressive:
            return ActionType::Attack;

        case AIState::Defensive:
        {
            int roll = randomInt(0, 1);
            if (roll == 0)
                return ActionType::Attack;
            if (canHeal)
                return ActionType::UseItem;
            return ActionType::Block;
        }

        case AIState::Desperate:
            return hasFocus ? ActionType::Attack : ActionType::Block;
    }

    return ActionType::Attack;
}

ActionType Enemy::decideAction()
{
    ai.update(
        get_hp(),
        hasItems(),
        has_focus()
    );

    return ai.decideAction();
}

void Enemy::info() const
{
    std::cout << get_name()
              << " HP: " << get_hp();

    if (get_focus() > 0)
        std::cout << " [Focus: " << get_focus() << "]";

    std::cout << std::endl;
}
