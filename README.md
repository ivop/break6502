# break6502

Gate level simulation of the 6502 CPU.  

The core code was lifted from https://github.com/emu-russia/breaks which was based on the extensive work at https://github.com/emu-russia/breaks.

This repo contains the CPU core, and SRAM, without all the NES and Board stuff, as an example how to use the core and test it for speed and accuracy.

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

Timed on an AMD FX(tm)-8320 Eight-Core Processor at 3.5GHz.  
6502 speed = 96247431 / 827 = 116381.4 = ± 116kHz  
