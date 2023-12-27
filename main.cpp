#include <stdio.h>
#include <iostream>
#include <string>
#include <list>
#include <map>
#include <cassert>
#include <memory.h>
#include <cstddef>

#include "Common/BaseLogicLib/BaseLogic.h"
#include "Chips/M6502Core/core.h"
#include "Common/BaseBoardLib/SRAM.h"

using namespace BaseLogic;

M6502Core::M6502 *core;
BaseBoard::SRAM *ram;

uint16_t addr_bus;
uint8_t data_bus;

TriState n_NMI;
TriState n_IRQ;
TriState n_RES;
TriState RDY;
TriState SO;
TriState PHI1;
TriState PHI2;
TriState RnW;
TriState SYNC;

TriState CLK = TriState::Zero;
TriState PrevCLK = TriState::X;

size_t phi_counter = 0;

M6502Core::UserRegs userRegs;

static void Step(void) {
    uint32_t ram_addr;
    bool dz = false;

    // Memory (Read)
    //
    if (RnW == TriState::One) {
        ram_addr = addr_bus;
        ram->sim(TriState::Zero, RnW, NOT(RnW), &ram_addr, &data_bus, dz);
    }

    // Core
    //
    TriState inputs[(size_t)M6502Core::InputPad::Max]{};
    TriState outputs[(size_t)M6502Core::OutputPad::Max];

    inputs[(size_t)M6502Core::InputPad::n_NMI] = n_NMI;
    inputs[(size_t)M6502Core::InputPad::n_IRQ] = n_IRQ;
    inputs[(size_t)M6502Core::InputPad::n_RES] = n_RES;
    inputs[(size_t)M6502Core::InputPad::PHI0] = CLK;
    inputs[(size_t)M6502Core::InputPad::RDY] = RDY;
    inputs[(size_t)M6502Core::InputPad::SO] = SO;

    core->sim(inputs, outputs, &addr_bus, &data_bus);

    PHI1 = outputs[(size_t)M6502Core::OutputPad::PHI1];
    PHI2 = outputs[(size_t)M6502Core::OutputPad::PHI2];
    RnW = outputs[(size_t)M6502Core::OutputPad::RnW];
    SYNC = outputs[(size_t)M6502Core::OutputPad::SYNC];

    // Memory (Write)
    //
    if (RnW == TriState::Zero) {
        ram_addr = addr_bus;
        ram->sim(TriState::Zero, RnW, NOT(RnW), &ram_addr, &data_bus, dz);
    }

    // Clock
    //
    CLK = NOT(CLK);

    if (IsNegedge(PrevCLK, CLK)) {
        phi_counter++;
    }

    PrevCLK = CLK;
}

static void printflags(M6502Core::UserRegs *ur) {
    printf("%c%c%c%c%c%c%c%c",
            ur->N_OUT ? '1' : '0',
            ur->V_OUT ? '1' : '0',
            true      ? '1' : '0',
            false     ? '1' : '0',
            ur->D_OUT ? '1' : '0',
            ur->I_OUT ? '1' : '0',
            ur->Z_OUT ? '1' : '0',
            ur->C_OUT ? '1' : '0');
}

static void load_test(const char *filename) {
    FILE *f = fopen(filename, "rb");

    if (!f) {
        fprintf(stderr, "error: unable to open %s\n", filename);
        exit(1);
    }

    for (int a=0; a<65536; a++) {
        int b = fgetc(f);
        if (b < 0) {
            fprintf(stderr, "error: premature EOF (%s)\n", filename);
            exit(1);
        }
        ram->Dbg_WriteByte(a, b);
    }

    fclose(f);
}

int main(int argc, char **argv) {
    printf("Starting Core... (first call generates Decoder6502.bin)\n");
    core = new M6502Core::M6502(false, false);     // HLE, BCD_HACK
    printf("Starting SRAM...\n");
    ram  = new BaseBoard::SRAM("RAM", 16, false);  // name, 1<<bits, trace

    load_test("6502_functional_test.bin");

    n_RES = n_NMI = n_IRQ = RDY = TriState::One;

    while (1) {
#if 0
        printf("Before STEP: AB: %04x DB: %02x CLK: %d ", addr_bus, data_bus, CLK);
        core->getUserRegs(&userRegs);
        printf("PC=%02x%02x A=%02x X=%02x Y=%02x S=01%02x ", userRegs.PCH, userRegs.PCL, userRegs.A, userRegs.X, userRegs.Y, userRegs.S);
        printflags(&userRegs);
        printf("\n");
#endif

        if (addr_bus == 0x3469) {
            printf("SUCCESS\n");
            printf("cycles = %ld\n", phi_counter);
            break;
        }

        Step();
    }

    delete core;
    delete ram;
}
