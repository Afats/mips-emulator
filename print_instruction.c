// z5232937, Mustafa Dohadwalla
// If you use C library functions add includes here.

#include "emu.h"
#include "ram.h"
#include "registers.h"
#include <stdlib.h>
#include <string.h>

/*
 * print a MIPS instruction (no newline)
 * run reference implementation if unsure what to print
 */

/*For each 32 bits:
1. Look at opcode to distinguish between R- Format, JFormat,
and I-Format
2. Use instruction format to determine which fields exist
3. Write out MIPS assembly code, converting each field to
name, register number/name, or decimal/hex number
4. Logically convert this MIPS code into valid C code.*/

void binary_display(int part, int *opcode) {

    //get bits in an array ---- LITTLE ENDIAN FORMAT
    int *bits = malloc(sizeof(int) * 6);

    for (int i = 5; i >= 0; i--) {
        bits[i] = part & 1;
        part >>= 1;
    }

    printf("\nBits: ");
    for (int i = 0; i <= 5; i++) {
        opcode[i] = bits[i];
        printf("%d", bits[i]);
    }

    printf("\n");

}


void binary_display_x(int part, int *opcode) {

    int *bits = malloc(sizeof(int) * 26);

    for (int i = 25; i >= 0; i--) {
        bits[i] = part & 1;
        part >>= 1;
    }

    printf("\nBits: ");
    for (int i = 0; i <= 25; i++) {
        opcode[i] = bits[i];
        printf("%d", bits[i]);
    }

    printf("\n");

}

// Binary display I value
void binary_display_I(int part, int *opcode) {

    int *bits = malloc(sizeof(int) * 26);

    for (int i = 15; i >= 0; i--) {
        bits[i] = part & 1;
        part >>= 1;
    }

    printf("\nBits: ");
    for (int i = 0; i <= 15; i++) {
        opcode[i] = bits[i];
        printf("%d", bits[i]);
    }

    printf("\n");

}

// Binary display of hex instruction
void binary_display_hexfull(uint32_t part) {
    
    //printf("\nHex value: %08x", part);

    //get bits in an array ---- LITTLE ENDIAN FORMAT
    // ---- FREE BITS ----

    int bits[32];

    for (int i = 0; i < 32; i++) {
        bits[i] = part & 1;
        part >>= 1;
    }

    printf("\nHex val: ");
    for (int i = 31; i >= 0; i--) {
        printf("%d", bits[i]);
    }

    printf("\n");

}

// seperate instruction into opcode, registers, etc
void print_instruction(uint32_t instruction) {
    
    // binary_display(instruction);

    //binary_display_hexfull(instruction);

    // opcode part 
    int part1 = (instruction >> 26);
    //int* opcode = malloc(sizeof(part1)*6);
    //binary_display(part1, opcode);
    //printf("\nPart 1: %d\n", part1);

    // rs & b part
    int part2 = (instruction & 0x03e00000) >> 21; 
    //binary_display(part2, opcode);
    //printf("Part 2: %d\n", part2);

    // rt part   
    int part3 = (instruction & 0x001f0000) >> 16;  
    //binary_display(part3, opcode);
    //printf("Part 3: %d\n", part3);

    // rd part     
    int part4 = (instruction & 0x0000f800) >> 11;     
    //binary_display(part4, opcode);
    //printf("Part 4: %d\n", part4);

    // shamt -- for shift function)            
    int part5 = (instruction & 0x000007c0) >> 6;
    //binary_display(part5, opcode);
    //printf("Part 5: %d\n", part5);        

    // funct part (arithmetic (R-type) part)
    int part6 = (instruction & 0x0000003f);  
    //binary_display(part6, opcode);
    //printf("Part 6: %d\n", part6);

    // I-type
    // address/immediate/offset
    int part7 = (instruction & 0x0000ffff);
    //int* I = malloc(sizeof(part1)*16);
    //binary_display_I(part7, I);
    //printf("Part 7: %d\n", part7);      

    // X part (immediate)                                        
    int part8 = (instruction & 0x03ffffff);
    //int* x = malloc(sizeof(part1)*26);
    //binary_display_x(part8, x);
    //printf("Part 8: %x\n", part8);   

    // -----------------------------------------------------

    //0xfc0007ff -- opcode mask

    // add instruction (rd = rs + rt)
    if ((instruction & 0xfc0007ff) == 0x00000020) {
        printf("add $%d, $%d, $%d", part4, part2, part3);
    }

    // sub instruction (rd = rs - rt)
    if ((instruction & 0xfc0007ff) == 0x00000022) {
        printf("sub $%d, $%d, $%d", part4, part2, part3);
    }

    // mul instruction (rd = rs * rt)
    if ((instruction & 0xfc0007ff) == 0x70000002) {
        printf("mul $%d, $%d, $%d", part4, part2, part3);
    }

    // and instruction (rd = rs & rt)
    if ((instruction & 0xfc0007ff) == 0x00000024) {
        printf("and $%d, $%d, $%d", part4, part2, part3);
    }

    // or instruction (rd = rs | rt)
    if ((instruction & 0xfc0007ff) == 0x00000025) {
        printf("or $%d, $%d, $%d", part4, part2, part3);
    }

    // xor instruction (rd = rs ^ rt)
    if ((instruction & 0xfc0007ff) == 0x00000026) {
        printf("xor $%d, $%d, $%d", part4, part2, part3);
    }

    // sllv instruction (rd = rs << rt)
    if ((instruction & 0xfc0007ff) == 0x00000004) {
        printf("sllv $%d, $%d, $%d", part4, part3, part2);
    }

    // srlv instruction (rd = rs >> rt)
    if ((instruction & 0xfc0007ff) == 0x00000006) {
        printf("srlv $%d, $%d, $%d", part4, part3, part2);
    }

    // slt instruction (rd = rs < rt)
    if ((instruction & 0xfc0007ff) == 0x00000002A) {
        printf("slt $%d, $%d, $%d", part4, part2, part3);
    }

    // Two's complement (print when required)

    // ~x+1=-x

    //int part7_twos_comp = (int16_t) part7;
    //printf("Twos comp: %d\n", part7_twos_comp);

    // addi instruction (rt = rs + I)
    if (part1 == 8) {
        printf("addi $%d, $%d, %d", part3, part2, (int16_t) part7);
    }

    // andi instruction (rt = rs & I)
    if (part1 == 12) {
        printf("andi $%d, $%d, %d", part3, part2, (int16_t) part7);
    }

    // ori instruction (rt = rs | I)
    if (part1 == 13) {
        printf("ori $%d, $%d, %d", part3, part2, (int16_t) part7);
    }

    // xori instruction (rt = rs ^ I)
    if (part1 == 14) {
        printf("xori $%d, $%d, %d", part3, part2, (int16_t) part7);
    }

    // sll instruction (rd = rt << I)
    if (part1 == 0 && part2 == 0 && part6 == 0) {
        printf("sll $%d, $%d, %d", part4, part3, part5);
    }

    // srl instruction (rd = rt >> I)
    if (part1 == 0 && part2 == 0 && part6 == 2) {
        printf("srl $%d, $%d, %d", part4, part3, part5);
    }

    // slti instruction (rt = rs < I)
    if (part1 == 10) {
        printf("slti $%d, $%d, %d", part3, part2, (int16_t) part7);
    }

    // lui instruction (rt = I << 16)
    if (part1 == 15 && part2 == 0) {
        printf("lui $%d, %d", part3, part7);
    }

    // lb instruction (rt = *(int8*)(b + O))
    if (part1 == 32) {
        printf("lb $%d, %d($%d)", part3, (int16_t) part7, part2);
    }

    // lh instruction 
    if (part1 == 33) {
        printf("lh $%d, %d($%d)", part3, (int16_t) part7, part2);
    }

    // lw instruction 
    if (part1 == 35) {
        printf("lw $%d, %d($%d)", part3, (int16_t) part7, part2);
    }

    // sb instruction 
    if (part1 == 40) {
        printf("sb $%d, %d($%d)", part3, (int16_t) part7, part2);
    }

    // sh instruction 
    if (part1 == 41) {
        printf("sh $%d, %d($%d)", part3, (int16_t) part7, part2);
    }

    // sw instruction 
    if (part1 == 43) {
        printf("sw $%d, %d($%d)", part3, (int16_t) part7, part2);
    }

    // beq instruction (if (s == t) PC += I<<2; else PC += 4;)
    if (part1 == 4) {
        printf("beq $%d, $%d, %d", part2, part3, (int16_t) part7);
    }

    // bne instruction 
    if (part1 == 5) {
        printf("bne $%d, $%d, %d", part2, part3, (int16_t) part7);
    }

    // blez instruction 
    if (part1 == 6) {
        printf("blez $%d, %d", part2, (int16_t) part7);
    }

    // bgtz instruction 
    if (part1 == 7) {
        printf("bgtz $%d, %d", part2, (int16_t) part7);
    }

    // bltz instruction 
    if (part1 == 1 && part3 == 0) {
        printf("bltz $%d, %d", part2, (int16_t) part7);
    }

    // bgez instruction 
    if (part1 == 1 && part3 == 1) {
        printf("bgez $%d, %d", part2, (int16_t) part7);
    }


    // j instruction 
    if (part1 == 2) {
        printf("j 0x%x", part8);
    }

    // jal instruction 
    if (part1 == 3) {
        printf("jal 0x%x", part8);
    }

    // jr instruction 
    if (part1 == 0 && part6 == 8) {
        printf("jr $%d", part2);
    }

    // syscall instruction 
    if (part1 == 0 && part6 == 12) {
        printf("syscall");
    }

}

// PUT YOUR FUNCTIONS HERE
