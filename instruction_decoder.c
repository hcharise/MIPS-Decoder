// Hannah Ashton

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

// Gets range of bits from word
unsigned extractor(unsigned input, int from, int to) {
    unsigned output = input << (31 - from);
    output = output >> (31 - (from - to));
    return output;
} // end extractor

// Gets rands of bits from word, returns as (signed) int
int int_extractor(unsigned input, int from, int to) {
    int output = input << (31 - from);
    output = output >> (31 - (from - to));
    return output;
} // end int_extractor


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

    // MY CODE HERE!!

    // Extract opcode
    unsigned opcode = extractor(instruction_binary, 31, 26);
    unsigned funct;

    // Print instruction name from opcode (and funct code if applicable)
    switch (opcode) {
        // R format
        case 0:
            funct = extractor(instruction_binary, 5, 0);
            switch (funct) {
                case 32:
                    printf("add ");
                    break;
                case 34:
                    printf("sub ");
                    break;
                case 42:
                    printf("slt ");
                    break;
                case 8:
                    printf("jr ");
                    break;
                case 0:
                    printf("sll ");
                    break;
                case 2:
                    printf("srl ");
                    break;
            }
            break;

        // J format
        case 2:
            printf("j ");
            break;
        
        // I formats
        case 8:
            printf("addi ");
            break;
        case 5:
            printf("bne ");
            break;
        case 4:
            printf("beq ");
            break;
        case 35:
            printf("lw ");
            break;
        case 43:
            printf("sw ");
            break;
    }

    // Array to decode registers
    const char *reg[] = {"$0", "$at",
                            "$v0", "$v1",
                            "$a0", "$a1", "$a2", "$a3",
                            "$t0", "$t1", "$t2", "$t3", "$t4", "$t5", "$t6", "$t7",
                            "$s0", "$s1", "$s2", "$s3", "$s4", "$s5", "$s6", "$s7",
                            "$t8", "$t9",
                            "$k0", "$k1",
                            "$gp", "$sp", "$fp", "$ra"};

    // Decode rs and rt registers, since used by many instructions
    unsigned rs = extractor(instruction_binary, 25, 21);
    unsigned rt = extractor(instruction_binary, 20, 16);

    // Print registers/constants
    if (opcode == 0) { // R FORMAT
        unsigned rd = extractor(instruction_binary, 15, 11);

        if (funct == 8) { // jr
            printf("%s", reg[rs]);
        } else if (funct == 0 || funct == 2) { // srl and sll
            unsigned shamt = extractor(instruction_binary, 10, 6);
            printf("%s, %s, %d", reg[rd], reg[rt], shamt);
        } else { // add, sub, and slt
            printf("%s, %s, %s", reg[rd], reg[rs], reg[rt]);
        }

    } else if (opcode == 2) { // J FORMAT, j
        unsigned address = extractor(instruction_binary, 25, 0);

        // Add two zeros at low end of address bit string
        address = address << 2;

        // Add four ones at high end of address bit string
        unsigned leading_ones = 0;
        int pos = 0;
        for (pos = 28; pos < 32; pos++) {
            leading_ones |= (1 << pos);
        }
        address |= leading_ones;
    
        printf("%u", address);

    } else { // I FORMAT
        // Extract immediate value as int
        int imm = int_extractor(instruction_binary, 15, 0);

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
