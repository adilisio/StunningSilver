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
4. **Silver identity.** **Lugia** is the story legendary, as in Silver/SoulSilver, with silver
   and ocean themes throughout the presentation. Ho-Oh moves to the post-game.

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

## 6. Quality of life

| Feature | Status | How |
|---|---|---|
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
| **M0 — Foundation** ✅ | Import hg-engine, branding/docs, badge-based level caps |
| **M1 — Playable build in CI** | Enable the upstream GitHub Actions build for this repo (it builds a HeartGold ROM from the pokeheartgold decomp and patches it). Produce a `.xdelta` patch artifact. |
| **M2 — Script source** | Dump scripts/event data into the repo (`BUILD_DUMPED_SCR_SEQ`, `BUILD_DUMPED_EVENTDATA`) so story edits are reviewable text. |
| **M3 — Johto encounters** | New wild encounter tables (`data/Encounters.c`, headbutt, surf, Safari/Bug Contest) with the expanded roster. |
| **M4 — Gyms 1–4 + early rivals/Rocket** | Boss teams, level-cap script bumps, Hardcore variants. |
| **M5 — Game-mode select** | New-game menu + flags for Hardcore / Restricted / Minimal Grinding. Implement the move/ability bans. |
| **M6 — Gyms 5–8, Lugia story swap, Elite Four** | Lugia replaces Ho-Oh in the story, plus E4 and Lance. |
| **M7 — QoL NPCs & key items** | Nature/IV/EV services, relearner, portable heal, HM-less field moves |
| **M8 — Kanto post-game + Red** | Kanto gyms, rematches, Mt. Silver |
| **M9 — Polish & release** | Rebalance pass, boss doc, Pokémon locations doc, patcher instructions |

## 9. Open decisions

1. Does the Silver identity go further than the Lugia story swap, e.g. a title screen and a palette/version byte change?
2. Is the starter selection the classic three, or an expanded choice?
3. Are Z-Moves, Dynamax or Terastallization in scope, or only Megas? hg-engine reserves flags for
   all of them, but their implementations depend on upstream.
4. The exact level-cap numbers: tune them after the M4 playtest.
