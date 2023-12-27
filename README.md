# break6502

Gate level simulation of the 6502 CPU.  

The core code was lifted from https://github.com/emu-russia/breaknes which was based on the extensive work at https://github.com/emu-russia/break.
Take a look at the 6502 Core book. Great stuff!

This repo contains the CPU core, and SRAM, without all the NES and Board stuff of breaknes, as an example of how to use the core and test it for speed and accuracy.
The accuracy test is the 6502 functional test by Klaus Dormann (https://github.com/Klaus2m5/6502_65C02_functional_tests) (CA65 version: https://github.com/amb5l/6502_65C02_functional_tests).

### Build Instructions

```
$ mkdir build
$ cd build
$ CXXFLAGS=-flto cmake ..
$ make -j8
$ cd ../test
$ time ../build/break6502
Starting Core... (first call generates Decoder6502.bin)
Starting SRAM...
SUCCESS
cycles = 96247431

real	13m47,725s
user	13m47,040s
sys	0m0,424s
```

### Speed

##### AMD FX8320 8-Core 3.5GHHz (Oct. 2012)
| LTO | HLE | time (s) | 6502 speed (kHz) |
| --- | --- | --- | --- |
| no | no | ±827 | ±116 |
| yes | no | ±184 | ±523 |
| yes | yes | ±175 | ±550 |

##### AMD Ryzen 5625U 6-Core 4.3GHz (Jan. 2022)
| LTO | HLE | time (s) | 6502 speed (kHz) |
| --- | --- | --- | --- |
| no | no | ±460 | ±209 |
| yes | no | ±80 | ±1203 |
| yes | yes | ±74 | ±1301 |

LTO (Link Time Optimization) inlines all the BasicLogic functions, which is a big improvement.
HLE (High Level Execution?) replaces some of the gate level logic with shorter and faster C code. Improvement is less than 10%.
I think the biggest speed gain was reached by replacing the PLA with a giant lookup table.
