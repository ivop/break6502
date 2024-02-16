cmake_minimum_required (VERSION 3.0)
project (break6502 CXX)

add_definitions (-D_LINUX)

# Main application

#set(CMAKE_BUILD_TYPE Release)

set(CMAKE_CXX_FLAGS "-flto -fomit-frame-pointer -march=native -Ofast -DNDEBUG")

add_executable (break6502 
	Common/BaseLogicLib/BaseLogic.cpp

    Common/BaseBoardLib/SRAM.cpp

	Chips/M6502Core/address_bus.cpp
	Chips/M6502Core/alu.cpp
	Chips/M6502Core/alu_control.cpp
	Chips/M6502Core/branch_logic.cpp
	Chips/M6502Core/bus_control.cpp
	Chips/M6502Core/core.cpp
	Chips/M6502Core/data_bus.cpp
	Chips/M6502Core/debug.cpp
	Chips/M6502Core/decoder.cpp
	Chips/M6502Core/dispatch.cpp
	Chips/M6502Core/extra_counter.cpp
	Chips/M6502Core/flags.cpp
	Chips/M6502Core/flags_control.cpp
	Chips/M6502Core/interrupts.cpp
	Chips/M6502Core/ir.cpp
	Chips/M6502Core/pc.cpp
	Chips/M6502Core/pc_control.cpp
	Chips/M6502Core/predecode.cpp
	Chips/M6502Core/random_logic.cpp
	Chips/M6502Core/regs.cpp
	Chips/M6502Core/regs_control.cpp

    main.cpp
)
