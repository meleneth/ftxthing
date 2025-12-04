bugfix combat - dead entities are still attacking
return to town should be rewarded - don't get loot / items unless you return to town?
ui: make log full-tall, display by default, and on the left 1/4 of the screen
ui: show player member wiget
sim: add gear
sim: add armor
sim: add cached 'total stats' -> equipped gear gives what stats? (likely exprTk)
sim: add 5 skill slots
sim: add combat planner
loot: add item drops
loot: add currency drops - field mouse drops $1
recruitment: add party member recruiter. $100 to recruit a level one with no skills.
recruitment: When the lowest member of a party attains level 10, a new player slot opens in that party until max 5
recruitment: when the party reaches level 25 (the party, not a player), a new party is unlocked until max 5

# Timed Battle

We should be able to TDD this thing pretty easily, so let's start with some easy tests and get the basic wiring in place

Currently, we have PartyLoop FSM that is dumping people into combat, when combat is over we go back to town, repair / restore hp/mp(?) regear and get ready to venture out

We have Encounter for storing details of who is in combat
we need a EventPP object on Encounter

we need per-combatant FSM

- Charging
- UsingSkill
- Casting
- GCD
- Queued
- Stunned
- Frozen
- Stopped (no actions, buffs / debuffs do not tick)
