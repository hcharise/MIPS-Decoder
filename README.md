# MIPS Decoder
This program converts binary strings of 32 bits into the corresponding MIPS assembly language instructions and registers. Written by Hannah Ashton in June 2023 using Visual Studio and the terminal for Syracuse's CPS 500 Computer Organization and Operating Systems.

Makefile, instruction_decoder.exe, and two sections (indicated in the comments) of the instruction_decoder.c file were provided by Professor William Katsak.

<img width="613" alt="Screenshot 2024-02-13 at 7 18 10 PM" src="https://github.com/hcharise/MIPS-Decoder/assets/110205350/5c3b667b-7b72-46d7-b84a-a09acaf7d173">


## Sample Input
00100011101111011111111111111100

10101111101100000000000000000000

00000000000001010100000010000000

00000001000001000100000000100000

# Overall Structure
This program takes in a 32-bit word in binary as a command-line argument. The program checks that the input was indeed a 32-bit word in binary, then parses it to be used by the remainder of the program. The "instruction_binary" variable represents the word and is gradually sectioned into the correct MIPS format, determining the instruction and registers, address, or immediate values, which are then printed back to the command line. The following format types and sections of each word are used:

![image](https://github.com/hcharise/MIPS-Decoder/assets/110205350/6d5b789e-b96f-4a53-a172-0a44ed97da70)
Credit: https://www.researchgate.net/figure/MIPS-instruction-format-18_fig1_220239180

## Instruction Decoding
To determine the instruction, the program first extracts the opcode, since this is a part of all MIPS instructions and indicates the format/later sections of the instruction. A switch statement then determines what instruction the opcode is indicating. If the opcode is the R format, then the function code is also extracted from the instruction_binary and switched on in order to narrow down to the specific function.

## Register Decoding
To print the registers, this program includes a reg[] array that lists all 32 registers in order. Since MIPS includes exactly 32 registers (and thus each register is represented by 5 bits, 0-31), the correct register can be identified simply by extracting the correct digits from the instruction_binary and using those to index into the reg[] array. For example, the $zero ($0) register is represented in MIPS binary as 00000, which equals 0 in decimal, so reg[0] is the $zero register.

## Addreses Decoding
Addresses in MIPS are represented with 32 bits, but the J format only has space for 26 bits dedicated to the address ("target address"), so we must adjust them accordingly. Memory addresses in MIPS occur every 4 bits, so the first two bits are insignificant; thus we adjust the 26 bits by inputting two "0"s to the low end (where they are insignificant). In a real MIPS decoder, the first four digits of the target address are copied from the current addreess, assuming that the jump will not be further away than this. For simplicities sake, this program assumees that these first four digits are "1"s.

## Immediate Decoding
The immediate values used in the I format (ex. addi) are extracted as integers and then printing.

# Program Functions and Descriptions
This program relies on the **extractor()** and **int_extractor** functions throughout main() to section the 32 bit word into the relevant sections. They take in "from" and "to" parameters, which represent bit numbers or indices, to indicate which section of the word should be returned. The unnecessary bits are removed using bit shifts to the left and the right (dropping off the extras).
