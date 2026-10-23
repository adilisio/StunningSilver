# StunningSilver — Design & Roadmap

> Radical Red's formula, applied to Johto.

## 1. Vision

Radical Red took FireRed's familiar Kanto and turned it into a hard, modern game. The roster
spans every generation, bosses play like competitive teams, and the friction of grinding is
gone. StunningSilver does the same for **Johto (and post-game Kanto)** on the
HeartGold/SoulSilver base.

### Pillars

1. **Familiar world, new fight.** Keep the Johto story, maps and music. Change *what you face*
   and *how battles play*.
2. **Every boss is a puzzle.** Gym leaders, rivals and Team Rocket admins have full
   competitive sets (EVs, natures, items, Megas) and smart AI. Losing should teach you something.
3. **Respect the player's time.** Hard level caps replace grinding. IVs/EVs, natures and moves
   are cheap to adjust. Healing, repels and running are always available.
4. **Silver identity.** **Lugia is the cover legendary**: it's on the title screen and box art,
   and it's the story legendary, as in Silver/SoulSilver, with silver and ocean themes throughout the
   presentation. Ho-Oh moves to the post-game.
5. **Same scope as Radical Red.** Where Radical Red has a feature (modes, QoL, Megas, Z-Moves,
   Dynamax raids, starter choice), StunningSilver has the Johto equivalent.

## 2. Platform decision

| | |
|---|---|
| Base game | Pokémon HeartGold (US, `IPKE`). hg-engine only supports HG. Version-exclusive content (Lugia story, SS-exclusive encounters) is moved over by editing data and scripts. |
| Engine | [hg-engine](https://github.com/BluRosie/hg-engine), with its history merged into this repo |
| Languages | C (`src/`, `data/*.c`), ARMIPS assembly (`armips/`, `asm/`), Python build scripts |
| Map/event editing | `data/eventdata`, `data/scr_seq` (enable `BUILD_DUMPED_EVENTDATA` / `BUILD_DUMPED_SCR_SEQ` once scripts are dumped with `make dump_scripts`), DSPRE for visual map work |

The alternatives considered were a pokecrystal-based GBC hack (too limited for a modern
roster) and pokeemerald-expansion on GBA (all of Johto would have to be rebuilt).

## 3. Game modes (chosen at new game)

These mirror Radical Red's modes. Each one is implemented as a save flag checked by
engine code and trainer data.

| Mode | Rules |
|---|---|
| **Normal** | StunningSilver bosses and badge-based level caps. |
| **Hardcore** | Harder boss teams. Selected route trainers become minibosses. Some boss fights have permanent field effects (e.g. Pryce: permanent Snow; Clair: permanent Tailwind on her side). The player is banned from weather-setting and most stat-boosting moves and abilities. Includes Minimal Grinding. |
| **Restricted** | Hardcore's player bans, with Normal bosses. |
| **Minimal Grinding** | All Pokémon have perfect IVs. EVs are disabled for everyone. Trading is disabled. |

Open question: should there also be a "Casual" mode with no caps and softer teams? Radical Red removed its Easy mode.

## 4. Level caps

Implemented as `LEVEL_CAP_FROM_BADGES` (`src/pokemon.c`, `sBadgeLevelCaps`). The cap comes from
Johto badges until the Hall of Fame, then from Kanto badges. Scripts can push it higher for
mid-badge bosses by setting var `0x416F`. The engine uses whichever is higher.

| Progress | Next major boss | Cap |
|---|---|---|
| 0 badges | Falkner | 13 |
| 1 | Bugsy | 19 |
| 2 | Whitney | 25 |
| 3 | Morty | 32 |
| 4 | Chuck | 38 |
| 5 | Jasmine | 44 |
| 6 | Pryce | 50 |
| 7 | Clair | 56 |
| 8 | Elite Four + Lance | 65 |
| Hall of Fame | 1st Kanto gym | 70 |
| +1 Kanto badge each | 2nd–8th Kanto gyms | 72 → 84 |
| 16 badges | Red (Mt. Silver) | 100 |

These numbers are a first pass and will be tuned once boss teams exist. Planned script bumps
(var `0x416F`) for mid-badge bosses:
- Rival at Azalea
- Team Rocket at Slowpoke Well
- Rival at the Burned Tower
- the Radio Tower takeover
- the Lugia encounter

## 5. Roster

### Starter selection ✅

Like Radical Red, the player chooses which starters to pick from. When you walk up to the starter
machine in Elm's lab, Elm asks which Pokémon you'd like to choose from:

| Option | Starters offered |
|---|---|
| **Johto** | Chikorita, Cyndaquil, Totodile |
| **Other regions** → Kanto, Hoenn, Sinnoh, Unova, Kalos, Alola, Galar, Paldea | That generation's grass/fire/water trio |
| **Random starters** | Each slot is the grass, fire or water starter of a random generation, e.g. Rowlet / Charmander / Piplup |
| **Truly random** | Any three different Pokémon out of all 1,025, legendaries included |
| **Random base stage** | Three different unevolved Pokémon that can evolve, excluding legendaries and mythicals (331 species, from Pichu and Eevee to Sprigatito) |

The rival (Silver) takes the starter in the slot that counters yours, as in vanilla, at the same
evolution stage. For example, with the Sinnoh set, choosing Turtwig means the rival takes
Chimchar, which becomes Monferno and then Infernape over the game. With either random option, he takes
whichever Pokémon sits in that slot. His trainer-data moves and ability are dropped for a swapped
starter, so it uses its own level-up moves.

How it works:
- `STARTER_SELECTION` in `include/config.h`, with set ids in `include/constants/starter_selection.h`
- the menu: Elm's lab script `data/scr_seq/scr_seq_00843_T20R0101.s` and text `data/text/543.txt`
- rolling the offers and swapping the rival's starter: `src/starters.c`, plus a hook in `src/field/enemy_party.c`
- vars `0x4160`–`0x4163`. The game still stores the Johto starter for the slot you took, so every
  vanilla rival script picks the right team.

Known gap: the starter screen's text (`data/text/190.txt`) no longer names the species, because
the hardcoded names would be wrong. You see the sprite and hear the cry. Showing the real name
needs a hook in the starter app's text printing, which requires a ROM to disassemble (after M1).

- **Johto dex expansion.** Johto's regional dex is extended with Pokémon from Gens 3–9 so that
  every route has several viable options. hg-engine currently covers the dex through Gen 6
  almost completely, with later gens in progress upstream. The roster grows as upstream lands species.
- **Availability philosophy** (from Radical Red): most Pokémon can be caught before the gym
  they would help against. Every type is available by Gym 3. Fully evolved "power" picks are gated
  behind the mid-game.
- **Rebalancing.** Deliberate buffs for weak Johto natives (Ledian, Ariados, Delibird, Dunsparce,
  Sunflora, Noctowl, Granbull, Corsola, Qwilfish, …) through stats, abilities and moves, the
  way Radical Red did. All changes go in `data/Species.c`, `data/learnsets/`, and `data/Moves.c`.
- **Megas.** Megas for Johto-themed Pokémon are a stretch goal; this means new sprites.

### Battle mechanics scope (same as Radical Red)

| Mechanic | Radical Red | StunningSilver status |
|---|---|---|
| Physical/special split, Fairy, Gen 9 moves & abilities | Yes | Available (hg-engine) |
| Mega Evolution / Primal Reversion | Yes | Available (hg-engine `MEGA_EVOLUTIONS`) |
| Z-Moves | Yes | **To build.** hg-engine has move data and `FLAG_Z_MOVE_ENABLED`, but no activation or move conversion yet. |
| Dynamax / Gigantamax raid dens | Yes (Wish Piece raids) | **To build.** hg-engine's Dynamax turn step is a `TODO` stub. Needs the battle mechanic, Max Moves, and raid den encounters. |
| Terastallization | No | Out of scope |

## 6. Quality of life

| Feature | Status | How |
|---|---|---|
| Starter selection (any generation or random) | **Done** | `STARTER_SELECTION`, see §5 |
| Hard level caps | **Done** | `IMPLEMENT_LEVEL_CAP` + `LEVEL_CAP_FROM_BADGES` |
| Rare Candy evolves at cap | **Done** | `ALLOW_LEVEL_CAP_EVOLVE` |
| EV/IV viewer on summary screen | On (upstream default) | `IMPLEMENT_NEW_EV_IV_VIEWER` |
| Reusable TMs | On (upstream default) | `REUSABLE_TMS` |
| Reusable repels (auto-prompt) | On (upstream default) | `IMPLEMENT_REUSABLE_REPELS` |
| 30 PC boxes | On (upstream default) | `EXPAND_PC_BOXES` |
| Items restored after battle | On (upstream default) | `RESTORE_ITEMS_AT_BATTLE_END` |
| EXP for captured Pokémon | On (upstream default) | `IMPLEMENT_CAPTURE_EXPERIENCE` |
| Deletable HMs | Todo | `DELETABLE_HMS` (needs field-move access without HMs first) |
| HM moves usable without knowing the move | Todo | new engine code |
| Nature changer / IV perfecter / EV reducer NPCs | Todo | scripts in Pokémon Centers / Goldenrod |
| Move relearner always available | Todo | script / party menu option |
| Portable heal ("PokéVial") and infinite repel toggle | Todo | key item + script |
| Fast text option | Todo | `FAST_TEXT_PRINTING` exists, but it's always-on; make it an option |

## 7. Boss design guidelines

- Every gym leader keeps their type theme but uses a full team of six from Gym 3 onward (fewer for early gyms).
- Sets are written like competitive sets: four purposeful moves, an item, nature and EVs. Use
  `data/Trainers.c`.
- Each boss has one clear "gimmick" the player must plan around, such as weather, Trick Room, a Mega,
  or a hazard stack.
- Rivals scale with your starter choice and always carry the starter Silver stole.
- Every boss team is documented in a boss doc for players, as Radical Red does.

## 8. Roadmap

| Milestone | Scope |
|---|---|
| **M0 — Foundation** ✅ | Import hg-engine, branding/docs, badge-based level caps, starter selection |
| **M1 — Playable build in CI** ✅ | `.github/workflows/build.yml` runs on every push: it builds HeartGold from the pokeheartgold decomp, runs hg-engine's battle tests, builds the playable ROM, and uploads only a `StunningSilver.xdelta` patch. First green run: 2026-10-23, about 73 minutes. |
| **M2 — Script source** | Dump scripts/event data into the repo (`BUILD_DUMPED_SCR_SEQ`, `BUILD_DUMPED_EVENTDATA`) so story edits are reviewable text. Individual scripts can already be taken over without a ROM. See "Editing a vanilla script" below. |
| **M3 — Johto encounters** | New wild encounter tables (`data/Encounters.c`, headbutt, surf, Safari/Bug Contest) with the expanded roster. |
| **M4 — Gyms 1–4 + early rivals/Rocket** | Boss teams, level-cap script bumps, Hardcore variants. |
| **M5 — Game-mode select** | New-game menu + flags for Hardcore / Restricted / Minimal Grinding. Implement the move/ability bans. |
| **M6 — Gyms 5–8, Lugia story swap, Elite Four** | Lugia replaces Ho-Oh in the story, plus E4 and Lance. |
| **M6b — Lugia cover** | Title screen with Lugia instead of Ho-Oh, and a StunningSilver logo. HG's title shows a 3D Ho-Oh model, and SoulSilver's assets can't be used from an HG base, so this needs original Lugia title art/model. Also the box art for the release page. |
| **M6c — Z-Moves & Dynamax** | Implement both in the battle engine (ideally contributed upstream to hg-engine), then Z-Crystals and raid dens. |
| **M7 — QoL NPCs & key items** | Nature/IV/EV services, relearner, portable heal, HM-less field moves |
| **M8 — Kanto post-game + Red** | Kanto gyms, rematches, Mt. Silver |
| **M9 — Polish & release** | Rebalance pass, boss doc, Pokémon locations doc, patcher instructions |

### Editing a vanilla script without a ROM

Script files named with a five-digit number (`data/scr_seq/scr_seq_NNNNN_*.s`) are always built,
and each one replaces that one script in the ROM. Elm's lab (script 843) was taken over this way:

1. Assemble the script from [pret/pokeheartgold](https://github.com/pret/pokeheartgold)
   (`files/fielddata/script/scr_seq/scr_seq_0843_T20R0101.s`). Check the binary against the
   SHA-1 in pokeheartgold's `scr_seq.sha1` to confirm it matches vanilla.
2. Decompile the binary with hg-engine's `NormalScriptParser` from
   `tools/source/dumptools/dump_event_scripts.py`. This gives the same format `make dump_scripts` writes.
3. Reassemble it with the build's flags and confirm it's byte-identical before making changes.
4. Text archives: convert pokeheartgold's `files/msgdata/msg/msg_NNNN_*.json` to
   `data/text/NNN.txt`. That's one message per line with CRLF endings, using `\n`, `\r` and `\f` escapes.

## 9. Decisions

Resolved:
- **Silver identity:** Lugia is the cover legendary (title screen and box art) and the story legendary.
- **Starters:** Radical Red-style choice of any generation, random starters, truly random, or random base stage (§5).
- **Mechanics scope:** the same as Radical Red. Megas, Z-Moves and Dynamax are in; Terastallization is out.

Open:
1. The exact level-cap numbers: tune them after the M4 playtest.
2. Should sub-legendaries that evolve (Type: Null, Kubfu, Poipole) be allowed in "random base
   stage"? They currently are, if hg-engine's baby table lists them as their own base.
