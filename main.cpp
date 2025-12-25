#include <iostream>
#include <string>
#include <random>

using namespace std;

// --------------------
// Random helper
// --------------------
int randomInt(int min, int max) {
    static mt19937 gen(random_device{}());
    uniform_int_distribution<> dist(min, max);
    return dist(gen);
}

// --------------------
// Base Entity class
// --------------------
class Entity {
protected:
    int hp;
    string name;

public:
    Entity(string n, int h)
        : hp(h), name(n) {}

    int get_hp() const {
        return hp;
    }

    string get_name() const {
        return name;
    }

    void take_damage(int dmg) {
        hp -= dmg;
        if (hp < 0) {
            hp = 0;
        }
    }

    virtual int get_attack_power() const {
        return 10;
    }

    int hit() {
        return randomInt(1, get_attack_power());
    }

    virtual void info() = 0;
};

// --------------------
// Player
// --------------------
class Player : public Entity {
protected:
    int weapon_bonus;

public:
    Player(string name, int hp, int weapon)
        : Entity(name, hp), weapon_bonus(weapon) {}

    int get_attack_power() const override {
        return 10 + weapon_bonus;
    }

    void info() override {
        cout << get_name() << " HP: " << get_hp() << endl;
    }
};

// --------------------
// Enemy
// --------------------
class Enemy : public Entity {
protected:
    int base_attack;
    int strength;

public:
    Enemy(string name, int hp, int baseAtk, int str)
        : Entity(name, hp),
          base_attack(baseAtk),
          strength(str) {}

    int get_attack_power() const override {
        return base_attack + strength;
    }

    void info() override {
        cout << get_name() << " HP: " << get_hp() << endl;
    }
};

// --------------------
// Battle
// --------------------
void Battle(Player& p, Enemy& e) {
    cout << "=== Battle Started ===" << endl;
    p.info();
    e.info();

    while (true) {
        int dmgToEnemy = p.hit();
        int dmgToPlayer = e.hit();

        e.take_damage(dmgToEnemy);
        cout << p.get_name() << " hits "
             << e.get_name()
             << " for " << dmgToEnemy << " damage" << endl;

        p.take_damage(dmgToPlayer);
        cout << e.get_name() << " hits "
             << p.get_name()
             << " for " << dmgToPlayer << " damage" << endl;

        cout << endl;

        p.info();
        e.info();
        cout << "-------------------------------------------" << endl;

        if (p.get_hp() == 0) {
            cout << endl;
            cout << "=== Battle Finished ===" << endl;
            cout << "Winner: " << e.get_name() << endl;
            break;
        }

        if (e.get_hp() == 0) {
            cout << endl;
            cout << "=== Battle Finished ===" << endl;
            cout << "Winner: " << p.get_name() << endl;
            break;
        }
    }
}

// --------------------
// Main
// --------------------
int main() {
    Player hero("Dark_Avanger", 100, 5);

    Enemy kobold("Sneaky_Kody", 50, 5, 3);
    Enemy orc("Gazkul_Trakka", 80, 7, 4);

    Battle(hero, orc);

    return 0;
}
