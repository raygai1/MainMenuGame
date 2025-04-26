#ifndef ENVIRONMENT_H
#define ENVIRONMENT_H
#include <iostream>
#include <random>
enum class EnvironmentType { //enumerators for different environment types
    Forest,
    Desert,
    Mountain,
    Village,
    City,
    Dungeon,
    Swamp,
    Cave,
    Ruins,
    Jungle,
    Tundra,
    Volcano
};
//declared inline function to get a random number in a range
inline int getRandomNumber(int min, int max) { //inline avoids linker errors
    static std::random_device rd;
    static std::mt19937 gen(rd());
    std::uniform_int_distribution<> distr(min, max);
    return distr(gen);
}
//structure holding environment data
struct EnvironmentData {
    EnvironmentType type;
    std::string description;
    std::string monsters[3];
    std::string boss[3];
};
//list of environments with descriptions and monsters
const EnvironmentData environments[] = {
    {EnvironmentType::Forest, "A dense forest with tall trees and thick underbrush.", {"Wolf", "Treant", "Bear"},{"Big Boy, Man Eater","Verdant Warden Carl","Elder Groveshade"}},
    {EnvironmentType::Desert, "A vast, scorching desert with dunes as far as the eye can see.", {"Scorpion", "Sand Wraith", "Mummy"},{"Malarath, the Eternal Dusk","Scourge of the Sands","Serpent Queen Sylestra"}},
    {EnvironmentType::Mountain, "A towering mountain with treacherous cliffs.", {"Mountain Goat", "Giant Eagle", "Rock Golem"},{"Khatta Duum, the Peak's Summit","Stonefury Gragorn","Howling Windshade"}},
    {EnvironmentType::Village, "A small, peaceful village with thatched-roof cottages.", {"Bandit", "Possessed Villager", "Cursed Blacksmith"},{"Betrayer Aldric","Haemonculi Revenant", "Plague Hermit Vaul"}},
    {EnvironmentType::City, "A bustling city filled with merchants and adventurers.", {"Thief", "Corrupt Guard", "Assassin"},{"RonaldMcDonald.TM","Steelborn Executioner","Shadow Baron Vexis"}},
    {EnvironmentType::Dungeon, "A dark, eerie dungeon full of unknown dangers.", {"Skeleton", "Demon", "Dark Sorcerer"},{"Warden of the Vaults","Chainscourge Festeris","The Masked Syndicate"}},
    {EnvironmentType::Swamp, "A murky swamp with mist covering the ground.", {"Giant Toad", "Swamp Wraith", "Lizard Man"},{"Zulvara the Mire Hag","Bog Tyrant Othrux","Slithermuck the Corrupter"}},
    {EnvironmentType::Cave, "A deep cave filled with the echoes of unseen creatures.", {"Bat Swarm", "Cave Troll", "Crystal Golem"},{"Hollowfang the Maw","Crystal Horror Myrix","Gloomlurker Senvir"}},
    {EnvironmentType::Ruins, "The ruins of an ancient civilization, crumbling and overgrown.", {"Ghost", "Ancient Guardian", "Shadow Knight"},{"Serathis, the Forsakn Herald","Ashen Monarch Velthra","Guardian of the Broken Seal"}},
    {EnvironmentType::Jungle, "A wild jungle teeming with exotic wildlife and hidden dangers.", {"Jaguar", "Venomous Snake", "Jungle Spirit"},{"Bloodroot Leviathan","Xela the Viper Priestess","Primal Lord Zangra"}},
    {EnvironmentType::Tundra, "A frozen wasteland covered in snow and ice.", {"Ice Wolf", "Frost Giant", "Yeti"},{"Frostborn Revenant","Solgrath Horror of the Tundra","Blight of the White Wastes"}},
    {EnvironmentType::Volcano, "A fiery volcano spewing lava and ash into the air.", {"Magma Beast", "Fire Elemental", "Lava Serpent"},{"Inferno Lord Drazhul","Magmaheart Gorves","Ember Seraph Kaida"}}
};
//function declarations
EnvironmentData generateEnvironment();
std::string determineMonster(const EnvironmentData& env, int luck);
#endif
