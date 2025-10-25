#include "encounter.hpp"

using namespace fairlanes;

Encounter::Encounter(Party &party) : party_(party) {}

void Encounter::tick(float dt) { (void)dt; }