FTXThing

Playing around w/EnTT, FTX::UI, boost_ext / SML and <strike>turn based</strike> combat systems

    cmake --build . --target clang-format

    find src -type f \( -name '_.cpp' -o -name '_.hpp' -o -name '_.c' -o -name '_.h' \) -exec clang-format -i {} +

This is the design doc at the moment, none of the below is implemented.

At the start, for dev purposes, this is scoped as an autobattler that the machine itself plays - this is basically a advanced version of Progress Quest.

The Loop:
Go to (Area) kill monsters get loot
return to town, heal up
craft gear, maintain gear
on level up, progress skills (assuming skills are level-cap based)
get consumables
goto 11

# Story
We start in a small village, and life is simple enough.   As we get to a certain level, the village is attacked and we must explore the larger world.

A core part of the progression is that we start in a small village, w/no magic - just normal swords / daggers / maces etc.  at 25% into the game, we start seeing magic.  at 50%, we start seeing modern weapons.  at 75%, we start seeing cyber + lazer weapons. (modern / magic introductions are still 'up in the air' for ordering)

This progression means that old defenses are invalidated as we move 'chapters'.  This implies at the least gearing resets, and possibly different sets of gear depending on what types of enemies you are expecting to encounter.

# Questions
How is area selected? is it level locked or ability to kill locked?
is creature selection biome based?
what damage types do we have?
what are caps for resistances?
what are the damage reduction formulas?

## how does experience work?
Level cap is 100
at level 1, you need to kill 10 things to get to level 2
at level 70, you would need to kill 710 things to get to level 71

## Biomes
Plains - Wolves, Horses, Meerkats
Tundra - Bison
Forest - Spiders, Squirrels, Wolves
Dark Forest - Snails
Jungle - Snakes, Gecko
Murky Swamp - Mosquitos, Dragonflies
Fetid Swamp - active rot - Mushrooms 
Oceanside - Crabs, Eels, Octopus
Caves - Bats, Blind Scorpions
Desert - Scorpions, Tarantula
Mines - Dark Bats, 
Ship - Narwahl, Swordfish, Kraken
Ice (berg? or just snowy outside?) - Yeti

# Drops
Loot tables are biome based
Each monster type has a specific rare drop
most monsters do not drop currency, but they do drop trash that merchants will buy
bag full = go sell

# Farming minigame
Grow reagents for consumables, seeds are purchasable or available as drops in certain biomes
plot size and time passing
mutations?

# Consumables
- Healing Potion (1 for each 10 levels)
- Mana Potion (1 for each 10 levels)
- Micro, Mini, Tiny, Little, Small, Plain, Extra, Large, Huge, Gigantic

# Entity Component Types

struct Name { std::string value; };
struct Health { int current, max; };
struct PlayerControlled {};
struct EnemyControlled {};
struct Level { int value; };

# Areas

## Starting Village

## Modern City

## Magic City

## Cyber City