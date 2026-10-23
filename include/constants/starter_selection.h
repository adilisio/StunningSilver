#ifndef STARTER_SELECTION_H
#define STARTER_SELECTION_H

// values for STARTER_SET_VARIABLE, shared between src/starters.c and the elm's lab script
#define STARTER_SET_DEFAULT          0 // sStarterChoices in src/starters.c
#define STARTER_SET_KANTO            1
#define STARTER_SET_JOHTO            2
#define STARTER_SET_HOENN            3
#define STARTER_SET_SINNOH           4
#define STARTER_SET_UNOVA            5
#define STARTER_SET_KALOS            6
#define STARTER_SET_ALOLA            7
#define STARTER_SET_GALAR            8
#define STARTER_SET_PALDEA           9
#define STARTER_SET_RANDOM_STARTERS 10 // each slot is the matching starter of a random generation
#define STARTER_SET_TRULY_RANDOM    11 // any three different species
#define STARTER_SET_RANDOM_BASE_STAGE 12 // three different unevolved species that can evolve, excluding legendaries and mythicals

#define NUM_STARTER_GENERATIONS (STARTER_SET_PALDEA)

#define STARTER_SLOT_GRASS 0
#define STARTER_SLOT_FIRE  1
#define STARTER_SLOT_WATER 2
#define NUM_STARTER_SLOTS  3

#endif // STARTER_SELECTION_H
