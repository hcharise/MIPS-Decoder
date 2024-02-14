/*
Hannah Ashton
Syracuse University
CPS 500 Computer Organization and Operating Systems
Professor William Katsak
June 2023
 
This program converts binary strings of 32 bits into the corresponding MIPS assembly language instructions and registers.
*/

// Section below (lines 11 - 47) provided by Professor WK
#include <stdio.h>
#include <string.h>

#define LENGTH 32
#define false 0
#define true 1

// Checks that the char array is 32 bits long
// and only has 0 and 1 in it.
int check(char* bits) {
    if (strlen(bits) != LENGTH) {
        return false;
    }
    for (int i = 0; i < LENGTH; i++) {
        if (bits[i] != '0' && bits[i] != '1') {
        return false;
        }
    }
    return true;
}

// Parse the char array (of '0' and '1' characters) into its
// unsigned binary equivalent.
unsigned parse_bits(char *bits) {
	unsigned result = 0;
	int i; 
	int pos = 0;
	for (i=strlen(bits)-1; i >= 0; i--) {
		if (bits[i] == '1') {
			result |= 1 << pos;
		}
		pos++;
	}
	return result;
}
// End section provided by Professor WK

// Gets specified range of bits from word by bit-shifting left then right
unsigned extractor(unsigned input, int from, int to) {
    unsigned output = input << (31 - from);
    output = output >> (31 - (from - to));
    return output;
} // end extractor

// Gets specified range of bits from word by bit-shifting left then right, returns as (signed) int
int int_extractor(unsigned input, int from, int to) {
    int output = input << (31 - from);
    output = output >> (31 - (from - to));
    return output;
} // end int_extractor

// Section below (lines 63 - 82) provided by Professor WK
// We will take the bitstring as a command line argument.
int main(int argc, char **argv) {
    // Make sure we have at least one argument.
    if (argc < 2) {
        fprintf(stderr, "No argument found\n");
        return -1;
    }

    // Get a pointer to the argument.
    char *instruction = argv[1];

    if (!check(instruction)) {
        fprintf(stderr, "Invalid bit string\n");
        return -2;
    }

    // Parse the argument (string) to unsigned (binary).
    unsigned instruction_binary = parse_bits(instruction);
    // End section provided by Professor WK

    // MY CODE

    // Extract the opcode from the input (opcode is bits 26-31)
    unsigned opcode = extractor(instruction_binary, 31, 26);
    unsigned funct;

    // Switch statement to determine and print the correct instruction using extracted opcode (and function code, if applicable)
    switch (opcode) {
        // R format
        case 0: // Opcode = 000000
	    // R format uses function code as bits 0-5, so extract that, then switch on funct's value
            funct = extractor(instruction_binary, 5, 0);
            switch (funct) {
                case 32: // Function Code = 100000
                    printf("add "); // Add
                    break;
                case 34: // Function Code = 100010
                    printf("sub "); // Subtract
                    break;
                case 42: // Function Code = 101010
                    printf("slt "); // Set if less than
                    break;
                case 8: // Function Code = 001000
                    printf("jr "); // Jump register
                    break;
                case 0: // Function Code = 000000
                    printf("sll "); // Shift left logical
                    break;
                case 2: // Function Code = 000010
                    printf("srl "); // Shift right logical
                    break;
            }
            break;

        // J format
        case 2:  // Opcode = 000010
            printf("j "); // Jump
            break;
        
        // I formats
        case 8: // Opcode = 001000
            printf("addi "); // Add immediate
            break;
        case 5: // Opcode = 000101
            printf("bne "); // Branch if not equal to
            break;
        case 4: // Opcode = 000100
            printf("beq "); // Branch if equal to
            break;
        case 35: // Opcode = 100011
            printf("lw "); // Load word
            break;
        case 43: // Opcode = 101011
            printf("sw "); // Store word
            break;
    }

    // Array storing all 32 MIPS register names
    // Since each register is represented in binary from 0 to 31, we can index into this array to find the corresponding register
    const char *reg[] = {"$0", "$at",
                            "$v0", "$v1",
                            "$a0", "$a1", "$a2", "$a3",
                            "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                            "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
                            "$t8", "$t9",
                            "$k0", "$k1",
                            "$gp", "$sp", "$fp", "$ra"};

    // Decode rs and rt registers, since used by many instructions
    // These extracted values index to the correct/correseponding register in the reg[] array
    unsigned rs = extractor(instruction_binary, 25, 21); // Register source, bits 21-25
    unsigned rt = extractor(instruction_binary, 20, 16); // Register operand, bits 16-20

    // Print registers names/constants
    if (opcode == 0) { // R FORMAT
        unsigned rd = extractor(instruction_binary, 15, 11); // R format also uses rd (register destination), bits 11-15

	// For each function, print registers in correct format
        if (funct == 8) { // jr
            printf("%s", reg[rs]);
        } else if (funct == 0 || funct == 2) { // srl and sll
            unsigned shamt = extractor(instruction_binary, 10, 6); // srl and sll also use shamt (shift amount), bits 6-10
            printf("%s, %s, %d", reg[rd], reg[rt], shamt);
        } else { // add, sub, and slt
            printf("%s, %s, %s", reg[rd], reg[rs], reg[rt]);
        }

    } else if (opcode == 2) { // J FORMAT, j (jump)
        unsigned address = extractor(instruction_binary, 25, 0); // j also uses an address, bits 0-25

	// MIPS addresses are actually 32 bits long, so we must convert the current "address" to the actual address
        // Input two zeros at low end of address bit string
        address = address << 2;

        // Input four ones at high end of address bit string
        unsigned leading_ones = 0;
        int pos = 0;
        for (pos = 28; pos < 32; pos++) {
            leading_ones |= (1 << pos);
        }
        address |= leading_ones;
    
        printf("%u", address);

    } else { // I FORMAT
        int imm = int_extractor(instruction_binary, 15, 0); // i format also uses an immediate value, bits 0-15

	// For each function, print registers in correct format
        if (opcode == 5 || opcode == 4) { // bne and beq
            printf("%s, %s, %d", reg[rs], reg[rt], imm);
        } else if (opcode == 35 || opcode == 43) { // lw and sw
            printf("%s, %d(%s)", reg[rt], imm, reg[rs]);
        } else { // addi
            printf("%s, %s, %d", reg[rt], reg[rs], imm);
        }

    }

    printf("\n");

    return 0;
}
