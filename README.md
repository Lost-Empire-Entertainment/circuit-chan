# Circuit Chan

Circuit Chan is a first-person puzzle/platformer game set in a research facility, inspired by Portal games for their level design and by Minecraft for its redstone system.  
The player character is a young woman equipped with a cybernetic arm, capable of precise manipulation of circuit components.  
Movement is free in first-person perspective, while interaction with puzzle elements occurs on a fixed 0.5m x 0.5m x 0.5m cubic grid.  
Progression is level-to-level, with each level containing a self-contained circuit challenge that must be completed to unlock the exit.

The game uses [KalaWindow](https://github.com/KalaKit/KalaWindow/tree/indev) as its foundation for windowing, graphics and input.

## Environment
Initial facility state is clean and operational:
- Uniform lighting
- Polished wall/floor panels
- Clearly organized wiring
- Distinct color-coded themes per room

Guidance is provided by an automated voice system with the following characteristics:
- Consistent tone and cadence
- Provides concise operational instructions
- No personality or emotional variance

Over time, environmental and system degradation occurs:
- Minor: flickering lights, delayed door responses, inconsistent indicator lighting
- Moderate: missing wall panels, exposed wiring, water leakage, irregular machine noise
- Severe: collapsed floor sections, jammed doors, partial room construction, emergency lighting only

## Circuit System

### Power Rules
- **Directional Flow:** Power transfers only through valid input/output faces
- **Power Decay:** Each block in the path consumes 1 unit of power
- **Source Strength:** Power sources output 15 units by default
- **Exact Requirements:** Target devices require an exact power amount to activate; surplus or deficit fails
- **Connection Integrity:** Blocks with both input and output require both to be connected for transfer

### State Rules
- State signals transmit via direct attachment to any side of a compatible block
- State does not carry power
- State blocks include **Activator** and **Memory**
- Memory blocks only accept state from an Activator
- State cannot affect pre-placed blocks or same-type state blocks

### Block Types
- **Power Blocks:** Wire, Layer Socket, Repeater, Power Switch, Split Switch, Delay, Inverter
- **State Blocks:** Activator, Memory
- **Special:** One-Way Socket (exact power target), Passthrough Socket (linked endpoint transfer)

### Placement Rules
- All placed blocks require solid ground
- Wires are non-solid; Layer Sockets can be placed on them
- Blocks are destroyed if supporting ground becomes non-solid

## Gameplay Progression
1. **Stable Operations:** Fully functional environment, clearly defined puzzles, standard block usage
2. **Anomalies:** Small inconsistencies appear, no effect on puzzle solvability
3. **Degradation:** Room construction irregularities, puzzle assembly using mismatched or repaired components
4. **Critical Failure:** Facility systems collapse, puzzles become improvised repairs to progress
5. **System Shutdown:** Guidance voice ceases operation, facility non-functional

## End State
Final chamber leads to an unsealed exit.  
Exterior environment is natural, with open sky and vegetation.  
Guidance system is inactive, and there are no signs of the original human operators.  
Player departs facility, marking the conclusion of operational scope.

---

# External libraries

These libraries are used by Circuit Chan for various purposes, their licenses are included in their folder inside '_external_shared/*' and the Circuit Chan license does not apply to them.

## Owned by Lost Empire Entertainment (under its KalaKit organization)

| Library                                                         | Use case                                                                     |
|-----------------------------------------------------------------|------------------------------------------------------------------------------|
| [KalaWindow](https://github.com/KalaKit/KalaWindow)             | Windowing, input, graphics and logging backend.                              |
| [KalaCrashHandler](https://github.com/KalaKit/KalaCrashHandler) | Able to detect crashes for various reasons that would otherwise cause a CTD. |

## Not owned by Lost Empire Entertainment

| Library                                                              | Use case                                             |
|----------------------------------------------------------------------|------------------------------------------------------|
| [GLM](https://github.com/icaven/glm)                                 | Math library for vectors, matrices, and 3D graphics. |
| [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) | Loads image files like PNG and JPEG into memory.     |
| [ImGui](https://github.com/ocornut/imgui)                            | Creates debug and editor user interfaces.            |
| [MiniAudio](https://github.com/mackron/miniaudio)                    | Plays and records audio on multiple platforms.       |
