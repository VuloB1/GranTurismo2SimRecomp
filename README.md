# GranTurismo2Sim Recompiled

<!-- retcomm-readme-metrics -->
[![GitHub downloads (all assets, all releases)](https://img.shields.io/github/downloads/VuloB1/GranTurismo2SimRecomp/total)](https://github.com/VuloB1/GranTurismo2SimRecomp/releases)
[![GitHub downloads (latest release)](https://img.shields.io/github/downloads/VuloB1/GranTurismo2SimRecomp/latest/total)](https://github.com/VuloB1/GranTurismo2SimRecomp/releases/latest)
[![GitHub release](https://img.shields.io/github/v/release/VuloB1/GranTurismo2SimRecomp)](https://github.com/VuloB1/GranTurismo2SimRecomp/releases/latest)
<!-- /retcomm-readme-metrics -->

Static recompilation of **Gran Turismo 2 (Simulation Mode, NTSC-U)** built on
[psxrecomp](https://github.com/mstan/psxrecomp) and
[recomp-ui](https://github.com/RetroPortingToolKit/recomp-ui).

| | |
|---|---|
| Disc | SCUS-94488 (Simulation Mode, v1.0) |
| Players | 2 |
| Region | USA |
| Publisher | Sony Computer Entertainment |
| Year | 1999 |

**Status: builds, not yet played through.** It is the Arcade project (which does
play from menus to a race) rebuilt for this disc, and I have not tested Simulation
mode myself yet. Expect problems. Gran Turismo 2 shipped as two
discs that boot different programs, so this repository covers only the **Simulation**
disc; the Arcade disc is the sibling project `GranTurismo2Recomp`. Both can share one
memory card.

This repository contains no game code or assets: you need your own legal disc
image, and the recompiled C is generated on your machine from it.

## Modern enhancements

Each one is a mod package (`mods/preloaded/packages/`) toggled in the
launcher's Mods page.

| Mod | State |
|---|---|
| Widescreen (`gt2.enhancement.widescreen`) | Native 16:9, on by default. Experimental: see known issues. |
| PGXP Precision (`psx.enhancement.pgxp`) | Sub-pixel vertex precision. **Off by default**, it costs roughly a quarter of the frame rate. `game.toml` also sets exact projection, depth buffer and a seam setting that removes lines across car bodies. |
| Skip FMVs (`gt2.enhancement.skip-fmvs`) | **Off by default.** The course-selection track previews are video streams too, so skipping makes them run several times too fast and breaks the music. |
| Analog pad | Default controller mode is analog, so the DualShock stick steers. |

Also set in `game.toml` / `CMakeLists.txt`: HLE textured batching (far fewer
draw calls on the course-selection map), `idle_skip`, and
`overlay_region_floor = 0x10000` so the race code, which loads inside the boot
program's address range, can run as native code.

## Known issues

- **Widescreen edges:** wall panels and objects near the camera that fall outside
  the original 4:3 frame are not drawn (the game cannot project geometry that
  straddles the camera plane), so cuts can show at the left/right edges. I looked
  for a draw-distance and edge-visibility lever in the game code and did not find
  one that changes this.
- **Draw distance** is the original game's; track chunks are streamed from disc.
- **First runs are slower.** Race code loads at run time as overlays. Until the
  overlay cache has compiled them to native code the game falls back to an
  interpreter. The runtime compiles them in the background while you play, with a
  short hitch (about a second) each time a batch lands. Set the environment
  variable `GT2_AUTOCOMPILE=1` to keep learning new code; without it the cache
  you already have is used and nothing is compiled in the background.
- Untested in play: licences, shop, championships. The overlay cache starts empty
  for this disc, so the first sessions run slowly while it fills.

## Building

Needs a C toolchain (built with the
[retcomm toolchain](https://github.com/RetroPortingToolKit/RetroPorting-Toolchains/releases/latest),
clang) on `PATH`, CMake, Ninja, Python and your own disc image.

```bash
git submodule update --init --recursive
bash psxrecomp/tools/ci/build_emitters.sh
python psxrecomp/psxrecomp_cli.py generate --config game.toml --project-root . --disc "/path/to/game.cue"
python psxrecomp/psxrecomp_cli.py rebuild --config game.toml --project-root . --build-dir build-release --no-pgo
```

`generate` writes about 1.2 GB of C into `generated/` (not committed) and the
build compiles it: expect a long first build. Point `[game] disc` in `game.toml`
at your cue, or choose the disc in the launcher.

For debugging, configure a second build with `-DPSX_DEBUG_TOOLS=ON` and drive it
over TCP with `tools/dbg.py` (`--debug-port 4370`).

<!-- retcomm-readme-launcher -->
## Retro Launcher

You can run this title **standalone** (download the release zip, point it at
your disc, play), or manage installs, updates, and disc/BIOS wiring with
**[Retro Launcher](https://github.com/RetroPortingToolKit/Retro-Launcher)** —
the Retro Compilation Manager hub for self-compiling recomps.

[Downloads](https://github.com/RetroPortingToolKit/Retro-Launcher/releases) ·
[Full README & features](https://github.com/RetroPortingToolKit/Retro-Launcher#readme)

<p align="center">
  <img src="https://raw.githubusercontent.com/RetroPortingToolKit/Retro-Launcher/main/docs/screenshots/hub-and-game-launcher.png" alt="Retro hub with a background build, next to a title’s recomp-ui launcher" width="720">
</p>

<p align="center">
  <img src="https://raw.githubusercontent.com/RetroPortingToolKit/Retro-Launcher/main/docs/screenshots/queue-and-background-build.png" alt="Background cmake build with titles queued" width="720">
</p>

Retro checks for updates, installs the prebuilt release zips, and automates
BIOS/ROM/save plumbing so you are not stuck repeating each game’s first run by hand.
<!-- /retcomm-readme-launcher -->

## Legal

You must own the original game. Disc images under `disc/` are gitignored and
must never be committed. Retail BIOS dumps are not redistributed and no C
derived from one may be committed; releases run on the bundled MIT OpenBIOS.

`generated/` (the recompiled game C) **is committed**: releases ship the
compiled game, built by CI from that tree. Regenerate and commit it whenever
seeds or the framework pin change.

Default app icon: `assets/psxrecomp.ico` (and `.png` / `.svg`) — Retro-themed controller mark from `psxrecomp/assets/`. Windows builds embed it via `APP_ICON`.

Optional box art under `launcher_assets/img/` may come from
[libretro-thumbnails](https://github.com/libretro-thumbnails/libretro-thumbnails)
(`Named_Boxarts`); see `BOXART_SOURCE.txt` when present.

## Quick start (dev)

```bash
git submodule update --init --recursive
./psxrecomp/tools/ci/build_emitters.sh
python3 psxrecomp/psxrecomp_cli.py generate \
  --config game.toml --project-root . --disc disc/<your>.cue
git add generated && git commit -m "Regenerate game C"
cmake -S . -B build-release -G Ninja -DCMAKE_BUILD_TYPE=Release
cmake --build build-release --target psx-runtime
```

Releases: tag `vX.Y.Z` (or run the *Release builds* workflow). CI builds the
committed `generated/` C on Linux, Windows and macOS and attaches
`gt2sim-<version>-<platform>.zip`, the compiled game. Locally:
`scripts/package_release.sh build-release linux-x64`.

## Symbols

Progressive map: `symbols.toml` → `python3 tools/sync_symbols.py` →
`psx_symbols.h` (`PSX_FN_*`). See `psxrecomp/docs/SYMBOLS.md`.

## Framework pins

Submodule gitlinks (`psxrecomp`, optional `recomp-ui`, nested `recomp-net`)
are authoritative. `framework_pins.txt` is an optional scaffold snapshot;
release CI logs SHAs with `record_pins.sh` but builds whatever the gitlinks
resolve to. Bump submodules deliberately — do not float on `main`/`master`
in release CI.

<!-- retcomm-readme-raid -->
---

<p align="center">
  <sub><b>R.A.I.D. — Retro AI Development</b> · a Discord for AI-assisted retro reverse-engineering, decomp &amp; recomp</sub>
</p>

<p align="center">
  <a href="https://discord.gg/Ad9BwSzctP"><img src=".github/raid-discord.png" alt="Join the Retro AI Development (R.A.I.D.) Discord" width="200"></a>
</p>
<!-- /retcomm-readme-raid -->
