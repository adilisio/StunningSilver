#include "../include/config.h"
#include "../include/constants/file.h"
#include "../include/constants/species.h"
#include "../include/constants/starter_selection.h"
#include "../include/pokemon.h"
#include "../include/save.h"
#include "../include/types.h"

extern u32 space_for_setmondata;
extern u32 sStarterChoiceCries[];

void LONG_CALL GetMonSpriteCharAndPlttNarcIdsEx(MON_PIC *picdata, u16 mons_no, u8 dir, u8 col, u8 form_no, u8 a5, u32 personality);

// Use MON_WITH_FORM(SPECIES_NAME, form) to specify a starter
// with a form.
//
// Keep in mind that these choices will impact Rival team
// determination in the appropriate trigger-scripts.
//
// The text during the starter-selection sequence does not name
// the species since STARTER_SELECTION changes them. That text
// is in data/text/190.txt.
static const u16 sStarterChoices[3] = {
    SPECIES_CHIKORITA,
    SPECIES_CYNDAQUIL,
    SPECIES_TOTODILE,
};

#ifdef STARTER_SELECTION

// the starter trio of each generation in grass, fire, water order, indexed by STARTER_SET_*
static const u16 sStarterSets[NUM_STARTER_GENERATIONS + 1][NUM_STARTER_SLOTS] = {
    [STARTER_SET_KANTO]  = { SPECIES_BULBASAUR,  SPECIES_CHARMANDER, SPECIES_SQUIRTLE },
    [STARTER_SET_JOHTO]  = { SPECIES_CHIKORITA,  SPECIES_CYNDAQUIL,  SPECIES_TOTODILE },
    [STARTER_SET_HOENN]  = { SPECIES_TREECKO,    SPECIES_TORCHIC,    SPECIES_MUDKIP },
    [STARTER_SET_SINNOH] = { SPECIES_TURTWIG,    SPECIES_CHIMCHAR,   SPECIES_PIPLUP },
    [STARTER_SET_UNOVA]  = { SPECIES_SNIVY,      SPECIES_TEPIG,      SPECIES_OSHAWOTT },
    [STARTER_SET_KALOS]  = { SPECIES_CHESPIN,    SPECIES_FENNEKIN,   SPECIES_FROAKIE },
    [STARTER_SET_ALOLA]  = { SPECIES_ROWLET,     SPECIES_LITTEN,     SPECIES_POPPLIO },
    [STARTER_SET_GALAR]  = { SPECIES_GROOKEY,    SPECIES_SCORBUNNY,  SPECIES_SOBBLE },
    [STARTER_SET_PALDEA] = { SPECIES_SPRIGATITO, SPECIES_FUECOCO,    SPECIES_QUAXLY },
};

// species 494-543 are the eggs and unused slots between arceus and victini
#define NUM_TRULY_RANDOM_SPECIES (SPECIES_ARCEUS + (MAX_CANONICAL_MON_NUM - SPECIES_VICTINI + 1))

/**
 *  @brief pick a random species out of every canonical species from bulbasaur to pecharunt
 *
 *  @return species id
 */
static u16 GetTrulyRandomSpecies(void)
{
    u32 index = gf_rand() % NUM_TRULY_RANDOM_SPECIES;

    if (index < SPECIES_ARCEUS) {
        return index + 1;
    }
    return index - SPECIES_ARCEUS + SPECIES_VICTINI;
}

/**
 *  @brief fill the starter offer vars from the set in STARTER_SET_VARIABLE if they are not filled yet
 *
 *  the elm's lab script clears the offer vars before the starter selection screen, so random sets are rolled exactly once
 */
static void FillStarterOffers(void)
{
    u32 set = GetScriptVar(STARTER_SET_VARIABLE);
    u16 offers[NUM_STARTER_SLOTS];
    int i, j;

    if (GetScriptVar(STARTER_OFFER_VARIABLE_START) != SPECIES_NONE) {
        return;
    }

    for (i = 0; i < NUM_STARTER_SLOTS; i++) {
        if (set >= STARTER_SET_KANTO && set <= STARTER_SET_PALDEA) {
            offers[i] = sStarterSets[set][i];
        } else if (set == STARTER_SET_RANDOM_STARTERS) {
            offers[i] = sStarterSets[gf_rand() % NUM_STARTER_GENERATIONS + STARTER_SET_KANTO][i];
        } else if (set == STARTER_SET_TRULY_RANDOM) {
            // reroll until the species differs from the ones already offered
            do {
                offers[i] = GetTrulyRandomSpecies();
                for (j = 0; j < i && offers[j] != offers[i]; j++);
            } while (j < i);
        } else {
            offers[i] = sStarterChoices[i];
        }
    }

    for (i = 0; i < NUM_STARTER_SLOTS; i++) {
        SetScriptVar(STARTER_OFFER_VARIABLE_START + i, offers[i]);
    }
}

/**
 *  @brief evolve a species through the first entry of its evolution table a number of times
 *
 *  @param species species id with the form in the upper bits like MON_WITH_FORM
 *  @param stages number of evolutions to go through
 *  @param heapID heap to load the evolution table on
 *  @return evolved species id with the form in the upper bits, stopping early at a species that does not evolve
 */
static u16 EvolveSpeciesByStages(u16 species, u32 stages, int heapID)
{
    struct Evolution *evoTable = sys_AllocMemory(heapID, MAX_EVOS_PER_POKE * sizeof(struct Evolution));

    for (; stages > 0; stages--) {
        ReadWholeNarcMemberByIdPair(evoTable, ARC_EVOLUTIONS, PokeOtherFormMonsNoGet(species & 0x7FF, species >> 11));
        if (evoTable[0].method == EVO_NONE) {
            break;
        }
        species = evoTable[0].target;
    }

    sys_FreeMemoryEz(evoTable);
    return species;
}

/**
 *  @brief replace a member of the rival's johto starter line with the offered starter in the same slot at the same evolution stage
 *
 *  the rival party picked by the trigger scripts has the johto starter that counters the player's slot (see the elm's lab script), so the slot carries over to the offered set
 *
 *  @param species species id from the trainer data, replaced if it is a johto starter
 *  @param form form from the trainer data, replaced alongside species
 *  @param heapID heap to load the evolution table on
 *  @return TRUE if the species was replaced; FALSE otherwise
 */
BOOL LONG_CALL SwapRivalStarterSpecies(u16 *species, u8 *form, int heapID)
{
    u32 slot, stage;
    u16 offer;

    if (*species < SPECIES_CHIKORITA || *species > SPECIES_FERALIGATR) {
        return FALSE;
    }

    slot = (*species - SPECIES_CHIKORITA) / 3;
    stage = (*species - SPECIES_CHIKORITA) % 3;
    offer = GetScriptVar(STARTER_OFFER_VARIABLE_START + slot);

    if (offer == SPECIES_NONE) {
        return FALSE;
    }

    offer = EvolveSpeciesByStages(offer, stage, heapID);

    if ((offer & 0x7FF) == *species && (offer >> 11) == *form) {
        return FALSE;
    }

    *species = offer & 0x7FF;
    *form = offer >> 11;
    return TRUE;
}

#endif // STARTER_SELECTION

/**
 *  @brief get the starter offered in a slot, including its form in the upper bits like MON_WITH_FORM
 *
 *  @param slot starter choice (0-2)
 *  @return species id with form
 */
static u16 GetStarterChoice(int slot)
{
#ifdef STARTER_SELECTION
    return GetScriptVar(STARTER_OFFER_VARIABLE_START + slot);
#else
    return sStarterChoices[slot];
#endif // STARTER_SELECTION
}

/**
 *  @brief fills the given array with the species ids of the starter choices
 *
 *  @param species pointer to the species array
 */
void LONG_CALL CreateStarter_SetStarterSpecies(int *species)
{
#ifdef STARTER_SELECTION
    FillStarterOffers();
#endif // STARTER_SELECTION

    for (int i = 0; i < 3; i++) {
        // strip off form
        species[i] = GetStarterChoice(i) & 0x7FF;
    }
}

/**
 *  @brief wrap the CreateMon call within CreateStarter to allow for starter forms
 *
 *  @param mon the PartyPokemon pointer
 *  @param species the species id
 *  @param slot starter choice (0-2)
 */
void LONG_CALL CreateStarter_CreateMon(struct PartyPokemon *mon, int species, int slot)
{
    u32 form = 0;

    if (slot >= 0 && slot < 3) {
        form = GetStarterChoice(slot) >> 11;
    }

    space_for_setmondata = form;
    PokeParaSet(mon, species, 5, 32, FALSE, 0, 0, 0);
    space_for_setmondata = 0;

    if (form != 0) {
        SetMonData(mon, MON_DATA_FORM, &form);
    }
}

/**
 *  @brief wrap the GetMonSpriteCharAndPlttNarcIdsEx call within createMonSprites to handle starter forms
 *
 *  @param pic MON_PIC pointer
 *  @param species species id
 */
void LONG_CALL CreateMonSprites_HandleForm(MON_PIC *pic, u16 species, u8 gender, u8 shiny, int slot)
{
    u32 form = 0;

    if (slot >= 0 && slot < 3) {
        form = GetStarterChoice(slot) >> 11;
        sStarterChoiceCries[slot] = (form == 0) ? species : PokeOtherFormMonsNoGet(species, form);
    }

    GetMonSpriteCharAndPlttNarcIdsEx(pic, species, gender, 2, shiny, 0, 0);

    if (form != 0) {
        GetOtherFormPic(pic, species, 2, shiny, form);
    }
}
