# Circuit System Summary

## Global Rules
- **Power Transfer:** Power only flows through valid input/output faces.  
  Non-power sides never transfer power, even if blocks are touching.

- **Power Decay:** Each wire or block in the path consumes 1 unit of power.  
  Power Sources start at 15 units; targets require an exact amount to activate.  
  Too low or too high fails activation.

- **Input/Output Completion:** Any block with both an input and an output requires both ends to be connected for power to transfer.  
  A disconnected input or output means no power flow.

- **Power Block Requirement:** A power block needs a powered input to send power through its outputs.

- **State Signals:** State travels via **attachment** (physical touch) on **any side** of the target.  
  State signals never carry power.

- **State Block Rules:**  
  - State Blocks = **Activator**, **Memory**  
  - Allowed targets for state = **Power Switch**, **Split Switch**, **Repeater**, **Activator**, **Delay**, **Memory**  
  - Same-type ban: State block does not affect other state block of the same type that is attached to it
  - **Memory** accepts state input **only** from an **Activator**  
  - Pre-placed blocks are **unaffected** by state signals  
  - State blocks have **no power outputs** and do **not** pass power through themselves

- **Placement:** All player-placed blocks must sit on solid ground (floor or another block).  
  **Wires are not solid** — only **Layer Sockets** can be placed on wires.  
  Player-placed blocks are destroyed **individually** if the ground beneath them becomes non-solid.

---

## Pre-Placed Blocks
- **Power Source**  
  Inputs = 0  
  Outputs = 1  
  - Always on; starts at 15 units of power  
  - Origin point for power distribution

- **One-Way Socket**  
  Inputs = 1  
  Outputs = 0  
  - Has an exact power requirement (shown next to it)  
  - Triggers a specific door, obstacle, or bridge

- **Passthrough Socket**  
  Inputs = 1  
  Outputs = 1 (directional)  
  - Accepts any power ≥ 1 unit  
  - Always placed in **paired** endpoints  
  - Carries power through walls, floors, or obstacles

---

## Player-Placed Blocks

### Power Blocks
- **Wire**  
  Inputs = 1  
  Outputs = 1  
  - Rotatable **straight** or **corner** piece (no T pieces)

- **Layer Socket**  
  Inputs = 1 (from existing wire/layer socket)  
  Outputs = 1 (to target layer)  
  - Accepts any power ≥ 1 unit  
  - Routes between grid layers  
  - Must be placed directly above/below an existing wire or layer socket

- **Repeater**  
  Inputs = 1 (minimum)  
  Outputs = 1  
  - Resets the **next wire** to full 15 units of power

- **Power Switch**  
  Inputs = 1  
  Outputs = 1  
  - Accepts any power ≥ 1 unit  
  - ON/OFF selects whether power passes  
  - Does not amplify signal

- **Split Switch**  
  Inputs = 1  
  Outputs = 2 (one active at a time)  
  - Accepts any power ≥ 1 unit  
  - **Requires both outputs to be connected** to function  
  - ON/OFF selects which output is active  
  - Does not amplify signal

- **Delay**  
  Inputs = 1  
  Outputs = 1  
  - Accepts any power ≥ 1 unit  
  - Modes = **0.5s**, **1s**, **2s**  
  - On rising edge: waits selected delay, outputs a single pulse (one tick); cancels if input drops before completion

- **Inverter**  
  Inputs = 1 (power)  
  Outputs = 1 (power)  
  - Accepts any power ≥ 1 unit  
  - Reverses ON/OFF condition of its output relative to its input  
  - Does not store state; transforms signal in real time

### State Blocks
- **Activator**  
  Inputs = 1 (power)  
  Control Link = 1 (to adjacent **allowed target**)  
  - Accepts any power ≥ 1 unit  
  - May attach on **any side** of the target (attachment sends **state only**)  
  - Sets the target’s ON/OFF condition to mirror the Activator’s condition  
  - **Normal mode:** powered = ON, unpowered = OFF  
  - **Reverse mode:** powered = OFF, unpowered = ON  
  - Requires continuous power to maintain its condition  
  - Control link never carries electrical power

- **Memory**  
  Inputs = 1 (state) — must come from an **Activator**  
  Control Out = 1 (to adjacent **allowed target**, optional)  
  - May attach on **any side** of both its input Activator and its output target (attachment sends **state only**)  
  - Starts **OFF**; toggles ON/OFF when the input Activator asserts a state change  
  - Holds state **without power**  
  - Output is optional; Memory functions even if nothing is attached
