#include "Items.h"
#include "Environment.h"

Item generateRandomItem() { //pulls a random item from the list
    int roll = getRandomNumber(0, 20); // updated upper bound
    switch (roll) {
        //+atk Weapons
        case 0: return Item("Sword", "A sharp blade.", ItemType::Weapon, getRandomNumber(1, 3));
        case 1: return Item("Axe", "Good for chopping.", ItemType::Weapon, getRandomNumber(1, 3));
        case 2: return Item("Mace", "Heavy and crushing.", ItemType::Weapon, getRandomNumber(1, 3));
        case 3: return Item("Golden Knife", "Shiny but deadly.", ItemType::Weapon, getRandomNumber(1, 3));
        case 4: return Item("A Gun", "Sorry Philosophy Majors, A Gun IS mightier than the pen AND sword.", ItemType::Weapon, getRandomNumber(1, 5));
        //+def Armor
        case 5: return Item("Iron Helmet", "Protects your head.", ItemType::Armor, getRandomNumber(1, 3));
        case 6: return Item("Chainmail", "Sturdy and reliable.", ItemType::Armor, getRandomNumber(2, 4));
        case 7: return Item("Boots", "Good for trekking.", ItemType::Armor, getRandomNumber(1, 2));
        case 8: return Item("Magic Cloak", "Mysterious defense aura.", ItemType::Armor, getRandomNumber(2, 5));
        case 9: return Item("Dragon Scale Armor", "Nearly impenetrable.", ItemType::Armor, getRandomNumber(3, 6));
        //-atk/def Cursed items
        case 10: return Item("Cursed Bow", "A bow that whispers to you.", ItemType::CursedWeapon, -getRandomNumber(1, 3));
        case 11: return Item("Cursed Gauntlet", "Heavy and draining.", ItemType::CursedArmor, -getRandomNumber(1, 3));
        case 12: return Item("Rusty Dagger", "It barely holds together.", ItemType::CursedWeapon, -getRandomNumber(1, 3));
        case 13: return Item("Broken Shield", "It offers no real protection.", ItemType::CursedArmor, -getRandomNumber(1, 3));
        case 14: return Item("Cursed Necklace", "Drains your will to fight.", ItemType::CursedArmor, -getRandomNumber(2, 4));
        //funny items=0
        case 15: return Item("A Rock!", "There is no better pet than A ROCK!", ItemType::Funny, 0);
        case 16: return Item("Plank", "Your new best friend!", ItemType::Funny, 0);
        case 17: return Item("Mysteriously Shaped Nuggets", "Is it chicken?", ItemType::Funny, 0);
        case 18: return Item("Whoopee Cushion", "Distract the enemy with this classic prank.", ItemType::Funny, 0);
        case 19: return Item("Fake Mustache", "Stylish, enemies might think you're someone else.", ItemType::Funny, 0);
        //gold+ reward
        case 20: return Item("Pile of Gold", "Some shiny coins!", ItemType::Gold, getRandomNumber(1, 5));
        default: return Item("Stick sword", "It looks like a sword.", ItemType::Weapon, 1);
    }
}
