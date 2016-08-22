# COP4-Disassembler
A disassembler for the National Semiconductor COP4 series

The program is in C.
It expects a binary file as input and attempts to derive COP400 code from the bits.
By default it assumes that the code starts at address 0x000 (the default for COP4 PROMS).
This can be changed using the -a address argument which changes the beginning address of the disassembled source.

The beginning address is very important for COP4 processors, as some opcodes in the region 0x080 - 0x0BF (ROM subroutines memory) have different meaning, then when found in the rest of the memory.

The address space of the COP400 is rather limited. It is 1024 bytes, divided into 16 pages of 64 bytes each.

Usage:
  cop4disas [-a address] binary
  The binary file contains the machine code we want to disassemble.
  The optional flag -a, allows us to specify a different start address for the disassembly.
  
  Bugs:
  The code produced is similar to the code expected by the NatSemi COP4 assembler.
  However there are differences (e.g. does not specify origin), so before feeding it to an assembler, some editting will be required.
  
  Having said that, I visually compared the code produced by cop4disas against COP4 code in NatSemi Application Notes, and it looks OK.
  
  
