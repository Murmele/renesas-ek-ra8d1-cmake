
Slint example running the RA8D1 board.

## Build

```bash
JLinkGDBServer -Device R7FA8D1BH -if SWD -speed 4000

# in another tab:
mkdir build
cd build
cmake ..
make -j24  && gdb-multiarch ra8d1_slint.elf -ex "target remote localhost:2331" -ex "load" -ex "monitor reset" -ex "continue"
```


## Prerequisites

```bash
# The rust toolchain and the right target
rustup target install thumbv8m.main-none-eabihf

# Gnu arm toolchain, eg:
sudo apt install gdb-multiarch  gcc-arm-none-eabi
```


## How was this made?

This can be seen in the git history of the project, but in sumarry:

 - The directories `ra`, `ra_cfg`, `ra_gen`, `script`, and `src` as well as the `memory_regions.lld` where
copied from an example project showing the screen.
 - The `CMakeLists.txt` was added to create the example
 - `slint-ra-fsp.cpp`/`slint-ra-fsp.h` contains the Slint platform integration
 - `main.cpp` and `app-window.slint` are the actual example files.
 - the GT911 driver header were adapted to compile in C++ mode
 - Some changes had to be done in bsp_cfg.h and new_thread0.c to increase the heap and stack size.
