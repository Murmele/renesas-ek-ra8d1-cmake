
Attempt to run a slint program on the RA8D1 board.

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
