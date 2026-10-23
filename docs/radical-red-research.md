# Pokémon Radical Red — Research Notes

Compiled 2026-10-23. Most fan sites (PokéCommunity, TV Tropes, fan wikis, trackers) were
blocked from the research environment. So this doc is built from search-result
summaries plus the GitHub READMEs of the engine projects. Facts marked
**(unverified)** came from one source only or from sources that disagree. Check
them against the official PokéCommunity thread or the Discord before relying on them.

---

## 1. What it is

| | |
|---|---|
| Type | Difficulty / "enhancement" ROM hack of **Pokémon FireRed** (GBA) |
| Authors | **soupercell** (lead) and **koala4** |
| Latest version | **v4.1**, released **2024-03-22**. No 4.2/5.0 had shipped as of mid-2026; a future "4.2/5.0" is only mentioned in community speculation. |
| Setting | Kanto (FireRed's story and map), plus the Sevii Islands and Johto-flavoured extra bosses (e.g. Falkner, Clair, Morty, Bugsy) |
| Pokémon | Up to **Gen 9**, including the Gen 9 DLC Pokémon (the v4.1 headline feature) |
| Official thread | PokéCommunity: "Pokémon Radical Red (VERSION 4.1 RELEASED! Gen 9 DLC Pokemon, Character Customization now available!)", thread id 437688 |

In short, it's FireRed's story with a modern battle engine (Gen 8–9 mechanics),
heavily rebalanced Pokémon, smart AI, competitive-style boss teams, and lots of
quality-of-life features.

## 2. Technical foundation (relevant if we build anything similar)

Radical Red is built on two open-source projects by **Skeli789**:

### CFRU — Complete FireRed Upgrade
<https://github.com/Skeli789/Complete-Fire-Red-Upgrade>
- Upgrades the battle engine to Gen 8: moves, abilities, items, new animations,
  improved AI, Mega Evolution, Z-Moves, Primal Reversion, Ultra Burst, hidden
  abilities, terrain, trainer EVs. Radical Red adds Dynamax on top.
- Extras: up to 25 PC boxes, move reminder with up to 50 moves, Battle Frontier,
  swarms, day/night/seasons, roamers, DexNav, a configurable start menu, a scrolling
  summary screen, and an updated daycare.
- Feature toggles live in `src/config.h`, switched by commenting and uncommenting lines.
- **Build:** put a clean FireRed US 1.0 ROM at `BPRE0.gba` and set `OFFSET_TO_PUT`
  in `scripts/make.py`. You need devkitPro/devkitARM on `PATH` and Python 3.6+. Run
  `python scripts/make.py`, which outputs `test.gba` and `offsets.ini`. Builds are incremental,
  but header changes need a clean build.
- Layout: `src/` (C), `assembly/`, `include/`, plus binary-hack folders
  `hooks/ functionrewrites/ repoints/ bytereplacement/`, and event scripting via
  XSE-compatible macros (`xse_commands.s`).

### DPE — Dynamic Pokémon Expansion
<https://github.com/Skeli789/Dynamic-Pokemon-Expansion>
- Expands species data in FireRed with dynamic insertion, so nothing needs manual repointing.
- Supports forms, gendered forms, per-form dex entries, up to 999 dex entries
  (forms not counted), up to 128 TMs/HMs, and 128 tutors.
- Insert **DPE first, then CFRU**. TM/tutor compatibility is defined per move
  (`src/tm_compatibility`, `src/tutor_compatibility`), not per species.

**Caveat:** Radical Red made additional *undocumented* structural changes to the
engine, so generic CFRU/DPE docs don't fully describe RR's ROM or save layout.
FireRed cheats mostly still work, but item and Pokémon modifier codes differ.

Community tooling that exists, all unverified and not reachable from here: a Radical Red save
editor (Caleb-Lite, its GitHub link now 404s), `vodiniz/radical_red_dex`, the
"Radical Dex" (radicaldex.pixelslug.eu, plus the dex.radicalred.net-style official dex).
There are also damage calculators, but I couldn't confirm a canonical repo.

## 3. Getting and running it

- **Base ROM:** *Pokémon FireRed (USA) v1.0*, the "1636 – Squirrels" dump (game code BPRE, rev 0).
  Patching **v1.1 white-screens**.
- Patch with the official online patcher, or apply the UPS/IPS patch manually.
  Clear your browser cache if the patcher serves an old version. Pre-patched ROMs are
  piracy, and the community won't share them.
- Emulators: **mGBA** (recommended in general) or VBA-M. Mobile users commonly use
  My Boy!/Pizza Boy (unverified).

## 4. Game modes (chosen at new game)

| Mode | What changes |
|---|---|
| **Normal** (default) | Standard RR bosses. Requires Brock/Misty/Surge rematches before Koga. |
| **Hardcore** | Boss fights much harder and "Kaizo-like". Some mandatory route trainers become **minibosses** (special battle transition). Some boss battles have **permanent weather/terrain**. The **player** is banned from many moves and abilities, mainly weather setters and most stat-boosting moves, with exceptions; the AI can still use them. Comes with Minimal Grinding automatically. Skips the rematch gate. |
| **Restricted** | Hardcore's move and ability bans, but with Normal bosses. |
| **Minimal Grinding** | All Pokémon have perfect IVs. **No EVs** for anyone, bosses included. **Trading disabled.** Doesn't change story, bosses, or caps. |
| Others | Older builds (v3.1) also offered an Easy difficulty and a **randomizer** in the start menu. Their current status is unverified. v4.1 added NG+ Mystery Gift codes, e.g. for monotype runs and better shiny odds. |

## 5. Level caps (v4.1)

RR enforces **hard level caps**: Pokémon stop gaining EXP at the next boss's cap.
Rare Candies can usually take you up to the cap. Caps differ between sources, and
these are the most consistent v4.1 numbers:

| # | Boss | Normal cap | Hardcore cap |
|---|---|---|---|
| 1 | Falkner / Brock (Pewter) | 15 | 16 |
| 2 | Archer (Mt. Moon) | 22 | 23 |
| 3 | Misty (Cerulean) | 27 | 28 |
| 4 | Lt. Surge (Vermilion) | 34 | 36 |
| 5 | Erika (Celadon) | 44 | 44 |
| 6 | Giovanni (Game Corner) | 47 | 47 |
| 7 | Silph Co. (Rival / Giovanni) | 56–57 | 56–57 |
| 8 | Sabrina (Saffron, **double battle**) | 59 | 59 |
| 9 | Koga (Fuchsia) | 68 | 68 |
| 10 | Rival / Blaine (Cinnabar, permanent Sun in Hardcore) | 73 / 76 | 73 / 76 |
| 11 | Archer, Ariana, Giovanni (Cerulean Cave) | 79–80 | 79–80 |
| 12 | Clair | 81 | 81 |
| 13 | Elite Four (2 team variants each) + Champion (Rival) | **85** | **85** |
| — | Post-game rematches (e.g. Morty) | up to 100 | |

Hardcore only differs from Normal at the first four caps. Hardcore gimmicks also
include permanent Sandstorm for Brock and permanent Rain plus a pre-transformed Palafin for Misty.
Brendan is an early optional or mini fight in Viridian Forest. Hardcore boss teams differ from Normal.

## 6. Mechanics and balance changes

- **Modern battle engine:** physical/special split, Fairy type, Gen 9 moves,
  abilities and items. Mega Evolution, Z-Moves, and Dynamax/Gigantamax (in raids via
  Wish Pieces). Mega/Z/Dynamax availability in boss fights varies.
- **Rebalancing:** nearly every Pokémon got some change, mostly buffs, with some
  nerfs and custom Megas (e.g. Floatzel gets Technician, Drapion gets Wicked Blow, and
  Mega Sceptile and Mega Garchomp stats were changed). v4.1 itself changed moves and
  abilities too: Forewarn was replaced with Toxic Chain, Supercell Slam got stronger,
  Shell Side Arm was reworked, and several moves got accuracy buffs.
- **AI:** competitive-style. It switches, predicts, and uses setup and weather. Bosses have
  full EVs, natures, items, and competitive sets. That's why players lean on damage
  calcs and the boss docs.
- **TMs are reusable**, and TMs and tutors were overhauled in v4.1. Weather TMs moved
  to a **weather tutor in Cinnabar**.
- Evolution methods were changed for trade evos and similar cases.

## 7. Quality-of-life features

- **IVs are shown** on the summary screen. There's an **IV perfecter** (Celadon,
  expensive), an **EV checker/reducer** (Saffron), and EV training gear.
- **Nature changer** in Pokémon Centers. There's also a move reminder and deleter, but their
  access point is unverified.
- **L-button "tools" menu:** auto-run, time changer, **infinite Repel**, and the
  **PokéVial** (a portable heal with limited charges).
- Expanded PC (CFRU), DexNav, a scrolling summary, and **character customization**
  via the dresser in your room (new in v4.1).
- Reusable TMs and many tutors.

## 8. Community and resources

- Official: PokéCommunity thread #437688, the Radical Red Discord, and r/radicalred.
- Official docs (Google Sheets/Docs): Pokémon locations, trainer and boss teams
  (Default and Hardcore Indigo League docs), and changes.
- Fan wiki: radicalred.miraheze.org. Trackers: radicalredtracker.com,
  nuzlocketracker.org, runlocke.com.
- Radical Dex: radicaldex.pixelslug.eu.
- **Watch out:** search results are full of auto-generated spam ("Radical Red Dex
  controversy", "31 legendary locations", claims that it's set in Hoenn or made by Nintendo).
  Don't trust them.

## 9. Open questions to verify with a human or another network setup

1. Exact v4.1 start-menu options: is the randomizer or Easy mode still present?
2. The full Hardcore move and ability ban list. The official doc has it.
3. Where the move reminder is and how many PokéVial charges you get.
4. Whether any 4.2/5.0 build or beta has been released since mid-2026.
5. The authoritative source for species, learnset, and trainer data, if we need machine-readable
   data. Candidates are the official dex's backing data and community GitHub repos.

## Sources

- CFRU README: https://github.com/Skeli789/Complete-Fire-Red-Upgrade
- DPE README: https://github.com/Skeli789/Dynamic-Pokemon-Expansion
- PokéCommunity official thread (incl. page 82, Hardcore/Minimal Grinding announcement): https://www.pokecommunity.com/threads/pok%C3%A9mon-radical-red-version-4-1-released-gen-9-dlc-pokemon-character-customization-now-available.437688/
- TV Tropes: https://tvtropes.org/pmwiki/pmwiki.php/VideoGame/PokemonRadicalRed
- PokéCoders (v4.1 info, cheats): https://www.pokemoncoders.com/pokemon-radical-red/ , https://www.pokemoncoders.com/pokemon-radical-red-cheats/
- Walkthrough / caps: https://pokefirered.com/pokemon-radical-red-walkthrough/ , https://radicalredtracker.com/bosses , https://nuzlocketracker.org/guides/radical-red , https://nuzlocketracker.org/guides/radical-red-hardcore , https://romhackguides.com/hacks/radical-red/ , https://romprofessor.com/radical-red/what-are-the-elite-four-and-champion-teams-in-pokemon-radical-red/
- Changes: https://www.pokeporto.com/pokemon-radical-red-all-pokemon-changes/ , https://www.pokeporto.com/pokemon-radical-red-changelog/
- Overviews: https://kotaku.com/games/pokemon-radical-red , https://www.hackdex.app/hack/radical-red , https://gbacodes.com/pokemon-radical-red/ , https://radicalred.miraheze.org/wiki/Main_Page
- RetroAchievements thread (4.2/5.0 speculation): https://retroachievements.org/forums/topic/15191?page=6
