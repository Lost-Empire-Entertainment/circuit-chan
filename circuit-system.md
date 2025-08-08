# Circuit System Summary

## Power System
- **Power Source**  
  Inputs = 0  
  Outputs = 1  
  - Pre-placed, single output, always on  
  - Starts at 15 units of power  
  - First connected wire gets full 15 units  

- **Power Decay**  
  - Each wire or block consumes 1 unit of power  
  - Targets require an exact amount; too low or too high fails activation  

---

## Pre-Placed Blocks
- **Power Source**  
  Inputs = 0  
  Outputs = 1  
  - Always on  
  - Starts at 15 units of power  
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
  - Always placed in paired endpoints  
  - Transfers power through walls, floors, or obstacles  

---

## Player-Placed Blocks  
All player-placed blocks must be placed on solid ground, either a floor or another block.  
Wires are not considered solid — only Layer Sockets can be placed on wires.  
Blocks are destroyed individually if the ground beneath them is not solid.

- **Wire**  
  Inputs = 1  
  Outputs = 1  
  - Rotatable straight or corner piece  

- **Layer Socket**  
  Inputs = 1 (from existing wire/layer socket)  
  Outputs = 1 (to target layer)  
  - Accepts any power ≥ 1 unit  
  - Allows routing to the grid layer above or below  
  - Must be placed directly above or below an existing wire or layer socket  

- **Repeater**  
  Inputs = 1 (minimum)  
  Outputs = 1  
  - Resets output to full 15 units of power on the next wire  

- **Power Switch**  
  Inputs = 1  
  Outputs = 1  
  - Accepts any power ≥ 1 unit  
  - Manual on/off toggle  
  - Does not amplify signal  

- **Split Switch**  
  Inputs = 1  
  Outputs = 2 (one active at a time)  
  - Accepts any power ≥ 1 unit  
  - Manual toggle to choose active output  
  - Does not amplify signal  

- **Activator**  
  Inputs = 1 (power)  
  Outputs = 0 (power)  
  Control Link = 1 (to adjacent switch)  
  - Accepts any power ≥ 1 unit  
  - Must be placed against a Switch Block or Double Switch Block (not on input/output sides)  
  - Toggles the target switch when powered  
  - Can operate in reverse mode to invert behavior  

---

## Visual Feedback
- Emissive textures indicate:  
  - Power level in 15 steps of brightness/color  
  - On/off or active/inactive state  
- No animations; changes update instantly on power change
