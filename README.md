# break6502

Gate level simulation of the 6502 CPU.  

The core code was lifted from https://github.com/emu-russia/breaknes which was based on the extensive work at https://github.com/emu-russia/break.
Take a look at the 6502 Core book. Great stuff!

This repo contains the CPU core, and SRAM, without all the NES and Board stuff of breaknes, as an example of how to use the core and test it for speed and accuracy.

```
$ mkdir build
$ cd build
$ cmake ..
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

| CPU | time (s) | 6502 speed (kHz) |
| --- | --- | --- |
| AMD FX8320 3.5GHz      | ±827 | ±116 |
| AMD Ryzen 5625U 4.3GHz | ±460 | ±209 |
