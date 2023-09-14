ROM_STARTING_ADDRESS = 0X8000                           # ROM Memory Address starts from 0x08000
RAM_STARTING_ADDRESS = 0x100                            # RAM Memory Address starts from 0x0100 to 0x03FF
RAM_INCREMENT_NO_BYTES = 8                              # Number of hexa values per line in RAM Memory
ROM_INCREMENT_NO_BYTES = 16                             # Number of hexa values per line in ROM Memory
STRING_FINAL_SPACE = "   "
ROM_OUTPUT_FILE_PATH = "D:/Desktop/ROM_Memory.txt"
RAM_OUTPUT_FILE_PATH = "D:/Desktop/RAM_Memory.txt"

# *****  Code for the ROM Memory  ***** #
# "EA" - hex for the NOP (no operation OPCODE)
# Populate all the memory with the default OPCODE
ROM_MEMORY = bytearray([0xea] * 32768)

# Our Program
# The following program will show few clocks, simple instruction:
# - load value 6 into the Accumulator A Register;
# - decrement te Accumulator value;
# - save Accumulator's value into RAM;
# - jump to a specific label - address;
# - execute the subroutine (load 0B value in A register, save it into RAM memory at the next address, return);
# - jump to the beginning of the program - enter in this infinite loop;

# "#" - immediate operand
# "$" - HEX value

# LDA #$06   -  Load the Accumulator (A register) with the next value 0x06
ROM_MEMORY[0] = 0xa9
ROM_MEMORY[1] = 0x06

# C6  -  Decrement the Accumulator -1
ROM_MEMORY[2] = 0xC6

# STA $100  -  Store the Accumulator to the following address
# Address $100 -> part of the RAM Memory
ROM_MEMORY[3] = 0x8D
ROM_MEMORY[4] = 0x00
ROM_MEMORY[5] = 0x01

# JSR $8077  -  Jump to Subroutine (JSR) to the label "lbl" (address 0x8077 -> little endian)
# At this step the microprocessor have to push the Program Counter into the RAM Memory
# At the end of this a RTS (Return from Subroutine) takes place - the processor have to remember where to return
ROM_MEMORY[6] = 0x20
ROM_MEMORY[7] = 0x77
ROM_MEMORY[8] = 0x80

# 4C $8000  -   Jump to the following address 0x8000
ROM_MEMORY[9] = 0x4C
ROM_MEMORY[10] = 0x00
ROM_MEMORY[11] = 0x80

# Subroutine:

# LDA $0B -> Load in Accumulator "B" value
ROM_MEMORY[119] = 0xA9
ROM_MEMORY[120] = 0x0B

# C6 -> Decrement the Accumulator -1
ROM_MEMORY[121] = 0xC6

# STA $0101 -> Store the Accumulator value - in RAM - at the following value
ROM_MEMORY[122] = 0x8D
ROM_MEMORY[123] = 0x01
ROM_MEMORY[124] = 0x01

# RTS -> Return from the subroutine (fetch the stored Program Counter address from the RAM Memory
ROM_MEMORY[125] = 0x60

# Reset Vector - used at the Start Up or after a Reset
# The beginning or the start of the code - first address read by the Program Counter - 0x8000
ROM_MEMORY[0x7ffc] = 0x00
ROM_MEMORY[0x7ffd] = 0x80

# *****  Code for the ROM Memory  ***** #


# Open a file for writing the ROM Memory
with open(ROM_OUTPUT_FILE_PATH, "w") as file:
    line = ""
    # Add memory line counter: HEX
    line += f"{ROM_STARTING_ADDRESS:08X}" + STRING_FINAL_SPACE
    byte_count = 0
    for byte in ROM_MEMORY:
        # Format each byte as a 2-character hexadecimal value with a space
        line += " " + f"{byte:02X}"
        # A new byte has been added -> increment counter
        byte_count += 1
        # Each address is 16 bytes size:
        if byte_count >= ROM_INCREMENT_NO_BYTES:
            file.write(line.strip() + "\n")
            ROM_STARTING_ADDRESS += ROM_INCREMENT_NO_BYTES
            # Reset
            line = ""
            line += f"{ROM_STARTING_ADDRESS:08X}" + STRING_FINAL_SPACE
            byte_count = 0

        # Write any remaining characters (less than 8) as a partial line
    if line:
        file.write(line.strip () + "\n")

    print(f"Result has been written to {ROM_OUTPUT_FILE_PATH}")


# Open a file for writing the RAM Memory
with open(RAM_OUTPUT_FILE_PATH, "w") as file:
    line = ""
    # Add memory line counter: HEX
    line += f"{RAM_STARTING_ADDRESS:08X}" + STRING_FINAL_SPACE
    byte_count = 0
    for byte in range(1024):
        # Format each byte as a 2-character hexadecimal value with a space
        line += " " + f"EA"
        # A new byte has been added -> increment counter
        byte_count += 1
        # Each address is 16 bytes size:
        if byte_count >= RAM_INCREMENT_NO_BYTES:
            file.write(line.strip() + "\n")
            RAM_STARTING_ADDRESS += RAM_INCREMENT_NO_BYTES
            # Reset
            line = ""
            line += f"{RAM_STARTING_ADDRESS:08X}" + STRING_FINAL_SPACE
            byte_count = 0

        # Write any remaining characters (less than 8) as a partial line
    if line:
        file.write(line.strip () + "\n")

    print(f"Result has been written to {RAM_OUTPUT_FILE_PATH}")