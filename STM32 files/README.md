# STM32 Project Files for teleoperation using IMU based gesture-recognition (Main Logic in `main.c`)

This repository is an STM32CubeIDE project.  
**To replicate the work, you only need one file: `Core/Src/main.c`, check out `Core/Inc/icm20948.c` for IMU macros in gloveTx.**

Everything else (`.project`, `.cproject`, `.mxproject`, CubeMX files, etc.) is standard STM32CubeIDE project scaffolding.
.ioc file can be set according to the controllers you are using.

---

## What to use from this repo

**Use this file only:**
- `Core/Src/main.c`

**Ignore these for replication:**
- `.settings/`, `.project`, `.cproject`, `.mxproject`
- `directionControl.ioc` / CubeMX metadata (unless you want to re-generate the project)
- Any IDE / workspace files

---

## Quick start (replicate in your own STM32CubeIDE project)

### 1) Create a fresh project
1. Open **STM32CubeIDE**
2. **File → New → STM32 Project**
3. Select your MCU / board (use the same one you used originally, if applicable)
4. Generate code as normal

### 2) Copy in the implementation
1. In your new project, navigate to:
   - `Core/Src/main.c`
2. Replace the entire file content with:
   - this repo’s `Core/Src/main.c`

That’s it — the logic and behavior are defined entirely in `main.c`.

---

## Notes (important)

- If your project uses different pins/peripherals/clock settings, update those **in CubeMX (.ioc)** or in the generated init functions — but keep the **core algorithm + main loop logic** from `main.c`.
- If `main.c` references external drivers (e.g., IMU, motor driver, UART helpers), ensure the corresponding `.h/.c` files exist in your project as well.  
  *If you only copy `main.c` and get missing includes, add the referenced driver files the same way you normally would.*

---

## Project structure (for clarity)

```text
directionControl/
├─ Core/
│  ├─ Inc/
│  └─ Src/
│     └─ main.c      ← only file needed to replicate
├─ .settings/
├─ .cproject
├─ .project
└─ *.mxproject / *.ioc (CubeMX metadata)
