# Slint Example Running on the RA8D1 Board

## Build

```bash
# https://www.segger.com/downloads/jlink/
JLinkGDBServer -Device R7FA8D1BH -if SWD -speed 4000

# in another tab:
cmake -S . -B build \
  -DCMAKE_TOOLCHAIN_FILE=toolchain-arm-none-eabi.cmake \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo
cd build
cmake ..
make -j24
# Fedora: `gdb`, Debian: `gdb-multiarch`
gdb-multiarch ra8d1_slint.elf -ex "target remote localhost:2331" -ex "load" -ex "monitor reset" -ex "continue"
```
## Prerequisites

```bash
# The Rust toolchain and the correct target
rustup target install thumbv8m.main-none-eabihf

# GNU Arm toolchain, e.g.:
# Debian:
# TODO: libnewlib-arm-none-eabi might be required as well
sudo apt install gdb-multiarch gcc-arm-none-eabi
# Fedora:
sudo dnf install arm-none-eabi-gcc-cs arm-none-eabi-gcc-cs-c++ arm-none-eabi-newlib
```

## How Was This Made?

This can be seen in the Git history of the project, but in summary:

* The directories `ra`, `ra_cfg`, `ra_gen`, `script`, and `src` as well as the `memory_regions.lld` file were
  copied from an example project demonstrating the screen.
* The `CMakeLists.txt` file was added to create the example.
* `slint-ra-fsp.cpp` / `slint-ra-fsp.h` contain the Slint platform integration.
* `main.cpp` and `app-window.slint` are the actual example files.
* The GT911 driver headers were adapted to compile in C++ mode.
* Some changes were made in `bsp_cfg.h` and `new_thread0.c` to increase the heap and stack size.
