# Pokémon StunningSilver

**StunningSilver** is a difficulty and enhancement hack of the Johto games. It takes
the concept of Radical Red, which modernized FireRed's Kanto, and applies it to
**Johto in HeartGold/SoulSilver**:

- modern battle mechanics (Fairy type, Mega Evolution, Gen 9 moves, abilities and items)
- an expanded and rebalanced roster
- hard level caps and competitive-style boss fights
- quality-of-life features that cut out grinding
- a Radical Red-style starter choice: any generation's trio, random starters, truly random, or random base stage

**Lugia** is the cover legendary.

It is built on [hg-engine](https://github.com/BluRosie/hg-engine), an engine overhaul for
English Pokémon HeartGold. hg-engine is to HGSS what CFRU/DPE are to FireRed.

- Design and roadmap: [`docs/design.md`](docs/design.md)
- Background research on Radical Red: [`docs/radical-red-research.md`](docs/radical-red-research.md)
- Engine documentation, setup and build instructions: [`docs/hg-engine-README.md`](docs/hg-engine-README.md), [`CONFIG.md`](CONFIG.md)

## Building

Follow the platform setup in [`docs/hg-engine-README.md`](docs/hg-engine-README.md). Then:

1. Dump your own **English (US) Pokémon HeartGold** cartridge (game code `IPKE`) and place
   it in the repository root as `rom.nds`. ROMs are not, and will never be, distributed here.
2. Run `make -j$(nproc)`. The output is `test.nds`.

## Differences from stock hg-engine

| Change | Where |
|---|---|
| Hard level cap that follows badge progress. Scripts can raise it further with var `0x416F`. | `LEVEL_CAP_FROM_BADGES` in `include/config.h`, table `sBadgeLevelCaps` in `src/pokemon.c` |
| Rare Candies can trigger an evolution at the level cap | `ALLOW_LEVEL_CAP_EVOLVE` in `include/config.h` |
| Elm asks which starters to offer: Johto, another region, random starters, truly random, or random base stage. The rival's starter follows suit. | `STARTER_SELECTION` in `include/config.h`, `src/starters.c`, `src/field/enemy_party.c`, `data/scr_seq/scr_seq_00843_T20R0101.s`, `data/text/543.txt` |
| Starter screen text no longer names the Johto starters | `data/text/190.txt` |

## CI

Every push runs `.github/workflows/build.yml`. It builds US HeartGold from the
[pokeheartgold](https://github.com/pret/pokeheartgold) decompilation, runs hg-engine's battle
tests, then builds StunningSilver. The run's artifact is `StunningSilver.xdelta`. Apply it to your
own US HeartGold dump with xdelta3 or an online xdelta patcher. Built ROMs are never uploaded.

## Updating the engine

hg-engine's history is merged into this repository, so upstream improvements can be pulled in with:

```sh
git remote add upstream https://github.com/BluRosie/hg-engine.git   # once
git fetch upstream main
git merge upstream/main
```

## Credits and license

StunningSilver is free and always will be. hg-engine's terms require that any hack built on it
involves **no money**, including optional donations. They also require its credits to be carried over.
See [`CREDITS.md`](CREDITS.md) and the disclaimer in [`docs/hg-engine-README.md`](docs/hg-engine-README.md).
Pokémon is © Nintendo / Creatures Inc. / GAME FREAK inc. This is an unofficial fan project.
