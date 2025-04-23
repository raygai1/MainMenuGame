#include "Items.h"
#include <random>
#include "Environment.h"

Item generateRandomItem() { //pulls a random item from the list
    int roll = getRandomNumber(0, 16);
    switch (roll) {
        //+atk weapons
    case 0: return Item("Sword", "A sharp blade.", ItemType::Weapon, getRandomNumber(1, 3));
    case 1: return Item("Axe", "Good for chopping.", ItemType::Weapon, getRandomNumber(1, 3));
    case 2: return Item("Mace", "Heavy and crushing.", ItemType::Weapon, getRandomNumber(1, 3));
    case 3: return Item("Golden Knife", "Shiny but deadly.", ItemType::Weapon, getRandomNumber(1, 3));
        // Armor (+defense)
    case 4: return Item("Iron Helmet", "Protects your head.", ItemType::Armor, getRandomNumber(1, 3));
    case 5: return Item("Chainmail", "Sturdy and reliable.", ItemType::Armor, getRandomNumber(2, 4));
    case 6: return Item("Boots", "Good for trekking.", ItemType::Armor, getRandomNumber(1, 2));
    case 7: return Item("Magic Cloak", "Mysterious defense aura.", ItemType::Armor, getRandomNumber(2, 5));
        //-atk/def cursed
    case 8: return Item("Cursed Bow", "A bow that whispers to you.", ItemType::CursedWeapon, -getRandomNumber(1, 3));
    case 9: return Item("Cursed Gauntlet", "Heavy and draining.", ItemType::CursedArmor, -getRandomNumber(1, 3));
    case 10: return Item("Rusty Dagger", "It barely holds together.", ItemType::CursedWeapon, -getRandomNumber(1, 3));
    case 11: return Item("Broken Shield", "It offers no real protection.", ItemType::CursedArmor, -getRandomNumber(1, 3));
        //funny items=0
    case 12: return Item("A Rock!", "There is no better pet than A ROCK!", ItemType::Funny, 0);
    case 13: return Item("Plank", "Your new best friend!", ItemType::Funny, 0);
    case 14: return Item("Mysteriously Shaped Nuggets", "Is it chicken?", ItemType::Funny, 0);
    case 15: return Item("Whoopee Cushion", "Distract the enemy with this classic prank.", ItemType::Funny, 0);
    case 16: return Item("Pile of Gold", "Some shiny coins!", ItemType::Gold, getRandomNumber(1, 5));
    default: return Item("Stick sword", "It looks like a sword.", ItemType::Weapon, 1);
    }
}
