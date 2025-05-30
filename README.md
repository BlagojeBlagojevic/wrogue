
# wrogue

A simple turn-based roguelike dungeon crawler written in C using SDL2, playable on desktop and in the browser (via Emscripten).

![game](https://github.com/user-attachments/assets/18032142-d44a-4512-b9e1-b348fc347464)


## Features

* Procedurally generated dungeons
* Turn-based movement and combat
* Permadeath
* Item discovery and inventory management
* Graphical display via SDL2
* Compile to native executables or WebAssembly for browser play

## Demo

Play the web demo [here](https://blagojeblagojevic.github.io/wrogue/) or build a native version.

## Requirements

* SDL2
* SDL2\_image
* SDL2\_ttf
* C compiler (GCC or Clang)
* GNU Make
* (Optional) Emscripten SDK for building the browser version

## Building and Running

### Native (Desktop)

1. Clone the repository:

   ```bash
   git clone https://github.com/BlagojeBlagojevic/wrogue.git
   cd wrogue
   ```

2. Build with Make:

   ```bash
   make build
   ```

3. Run the game:

   ```bash
   ./app
   ```

> *Note:* On Windows, use the provided `SDL2.dll`, `SDL2_image.dll`, and `SDL2_ttf.dll` in the project root.

### Web (Emscripten)

1. Ensure Emscripten SDK is installed and activated.

2. From the project root, build the HTML version:

   ```bash
   make buildemcc
   ```

3. Serve the `html` with a local web server:

   ```bash
   python3 -m http.server 8080
   ```

4. Open `http://localhost:8080/` in your browser.

For detailed Emscripten instructions, see [`emcc.md`](emcc.md).

## Controls

| Key                   | Action           |
| -----------------     | ---------------- |
| Arrow Keys / WASDQEZC | Move             |
| Space                 | Wait / Pass turn                              |
| `p`                   | Pick up item                                  |
| `u`                   | Open inventory and type what you want to use  |

## Configuration

There are no external configuration files. To tweak settings (map size, spawn rates, etc.), edit constants in `utils.h` and rebuild.

## Project Structure

```
├── assets/          # Game assets (sprites, fonts)
├── app.c            # Main application loop and SDL2 setup
├── entity.c/.h      # Entity definitions and behavior
├── map.c/.h         # Dungeon generation and map handling
├── item.c/.h        # Item definitions and inventory logic
├── utils.h          # Utility macros and constants
├── makefile         # Build targets for native and web
├── emcc.md          # Emscripten build instructions
└── README.md        # This document
```

## Roadmap

* [x] Monsters
* [x] Implement procedural item descriptions
* [x] Improve graphical tileset and UI
* [x] Bsp map generation
* [x] Drunk walk map generation
* [ ] Brouge style map genration  
* [x] Potion systems
* [x] Scrol systems
* [X] Range combat
* [ ] Wands
* [ ] Rings
* [ ] Balance
* [ ] More monsters
* [ ] Traps
* [ ] Raycaster engine 

## Monsters

A variety of creatures roam the dungeons of wrogue, each presenting unique challenges:

### Undead

* **Acolyte**
  A nimble caster with modest health  and balanced defenses. Uses basic attacks and dark spells to harass adventurers.
* **Zombie**
  Slow-moving undead brutes that relentlessly hunt intruders. Their basic attacks deal minor damage, but their berserk tendency makes them unpredictable when injured. Occasionally siphons life to sustain its shambling form. Spell poisons gas.
* **Ghoul**
  Ferocious undead predators with powerful basic strikes. More aggressive than zombies, ghouls relentlessly chase prey and gain strength by draining life with each successful hit
* **Abomination**
  A grotesque amalgamation that spews poison clouds instead of direct blows. High poison attack makes it a dangerous crowd controller in tight corridors.
  Spell poisons gas
  
* **Wagon**
  A haunted war wagon  that cannot attack directly but casts summoning spells. 
* **Necromancer**
  Frail spellcaster with light defenses. Prefers to summon ghouls from the shadows and maintain distance. Fleeing is common unless bolstered by summoned minions.
* **Banshee**
  An ethereal wailer whose piercing shriek drains vitality. Combines poison attacks with debilitating curse spells that reduce the hero’s maximum health.
* **Spider**
  A venomous cave crawler that fires poisonous webs at range. Agile and persistent, spiders avoid close combat unless cornered and may bind prey in sticky snares.
* **Gargoyle**
   Stone-skinned sentinels with balanced melee and ranged prowess. Tireless watchers that swoop in for crushing blows or hurl stones from atop ledges.
* **Destroyer**
  A hulking spellblade blending basic strikes with destructive magic. Wields devastating offensive spells, making it deadly at any encounter distance.
  
* **Dragon**
  The apex predator of the dungeons. Masters both melee and ranged fire attacks. High stamina and spell likelihood allow it to dominate prolonged battles.

### Orcs

* Grunt 
* Berserker 
* Archer 
* Witch 

### Neutral
* Rat 
* Goblin 




## License

This project is licensed under the MIT License. See the [LICENSE](LICENSE) file for details.



