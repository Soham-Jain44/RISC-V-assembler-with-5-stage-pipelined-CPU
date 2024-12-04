# RISC-V Assembler and CPU Simulator

This project simulates a RISC-V assembler and CPU, where instructions are parsed, translated to machine code, and then executed in a 5-stage pipelined CPU simulation.

## Features:
- Converts RISC-V assembly instructions into machine code.
- Simulates a 5-stage pipelined CPU with Fetch, Decode, Execute, Memory Access, and Write Back stages.
- Implements control logic for various instruction types: R-type, I-type, S-type, B-type, U-type, and J-type.
- Detects pipeline hazards and manages stall cycles.
- Simulates the execution of instructions with accurate memory and register file operations.

## Instruction Set Supported:
The following RISC-V instructions are supported:
- **R-type Instructions**: `add`, `sub`, `sll`, `slt`, `xor`, `srl`, `or`, etc.
- **I-type Instructions**: `addi`, `lw`, `xori`, `jalr`, etc.
- **S-type Instructions**: `sb`, `sw`, `sd`.
- **B-type Instructions**: `beq`, `bne`, `blt`, `bge`, etc.
- **U-type Instructions**: `lui`, `auipc`.
- **J-type Instructions**: `jal`.

## Structure:

### 1. **Instruction Parsing and Machine Code Generation**:
   - The `parsing()` function converts RISC-V assembly instructions into their machine code format.
   - Instruction format is identified using the `formatFinder()` function, which assigns each instruction to one of six types: R, I, S, B, U, and J.

### 2. **5-Stage Pipeline Simulation**:
   - The pipeline consists of five stages:
     - **Fetch (IF)**: Fetches instructions from memory.
     - **Decode (ID)**: Decodes the instruction and reads registers.
     - **Execute (EX)**: Executes the instruction, computes the ALU result.
     - **Memory Access (MEM)**: Performs memory read or write.
     - **Write Back (WB)**: Writes the result to the register file.
   - The `run_pipeline()` function runs the pipeline in a loop until all instructions are processed.

### 3. **Hazard Detection and Stalls**:
   - The pipeline includes logic for detecting data hazards and controlling pipeline stalls.
   - When a hazard is detected (e.g., a register value is needed before it's available), the pipeline stalls to wait for the result to be ready.

### 4. **ALU Operations**:
   - The `ALUControl()` function determines the ALU operation based on the opcode, `funct3`, and `funct7`.
   - The `ALU()` function performs arithmetic and logical operations based on the selected operation.

### 5. **Memory and Register Operations**:
   - **Memory**: A data memory (`DM`) is used to simulate memory read/write operations.
   - **Registers**: The general-purpose registers (`GPR`) are updated based on instruction results.

### 6. **Helper Functions**:
   - Functions like `signedExtend()`, `extractIMM()`, and `extract_imm_S()` handle immediate value extraction and sign extension for various instruction formats.

### 7. **Simulation Control**:
   - The `clear_*()` functions reset pipeline stage registers and flags.
   - The `printGPR()` and `printDM()` functions display the final state of the general-purpose registers and data memory.

## Usage:

### Input:
1. The user is prompted to enter RISC-V assembly instructions line-by-line.
2. An empty line marks the end of input.

### Output:
- **Machine Code**: Displays the machine code generated for the input instructions.
- **Registers (GPR)**: Shows the final state of the general-purpose registers.
- **Memory (DM)**: Displays the memory contents (addresses and data).
- **Statistics**: Displays the total number of instructions executed, the number of cycles, and the calculated CPI (Cycles Per Instruction).
## Files:
- **`current.cpp`**: The main program that handles input, parsing, and simulation.
- **`README.md`**: This documentation file.

## Dependencies:
- C++11 or later
- No external libraries required (standard C++ library used).

## Future Improvements:
- Handle more RISC-V instructions.
- Implement forwarding to reduce stall cycles.
- Add support for exceptions and interrupts.
