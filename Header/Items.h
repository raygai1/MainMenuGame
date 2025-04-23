#ifndef ITEMS_H
#define ITEMS_H

#include <iostream>
#include <string>
#include <vector>
#include <random>

enum class ItemType {
    Weapon, //+atk
    Armor, //+def
    Funny, //fun items with only entertainment value=0
    CursedWeapon, //-atk
    CursedArmor, //-def
    Gold //++gold
};
struct Item {
    std::string name; //item name
    std::string description; //item description
    ItemType type; //category
    int statEffect; //+-atk or +-def depending on type
    //constructor for items
    Item(std::string name, std::string desc, ItemType type, int effect)
        : name(std::move(name)), description(std::move(desc)), type(type), statEffect(effect) {}
};
//stream output for item object
inline std::ostream& operator<<(std::ostream& os, const Item& item) {
    os << item.name << " (Effect: " << item.statEffect << ")";
    return os;
}
Item generateRandomItem(); //generates a random item from the list

#endif
