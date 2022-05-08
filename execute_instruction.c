// z5232937, Mustafa Dohadwalla
// If you use C library functions add includes here.

#include "emu.h"
#include "ram.h"
#include "registers.h"
#include <stdlib.h>
#include <string.h>

/**
 * execute a MIPS instruction
 *
 * This function should:
 *
 * get register values by calling `get_register(register_type register_number)`
 * change registers by calling `set_register(register_type register_number, uint32_t value)`
 *
 * get memory values by `calling get_byte(uint32_t address)`
 * changes memory by `set_byte(uint32_t address, uint8_t value)`
 *
 * updates program_counter to address of next instruction
 *
 * returns 1 if an exit syscall is executed, 0 otherwise
 */
int execute_instruction(uint32_t instruction, uint32_t *program_counter) {


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

    // I-type and O
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

    // 0xfc0007ff -- opcode mask

    // add instruction (rd = rs + rt)
    if ((instruction & 0xfc0007ff) == 0x00000020) {
        uint32_t x = get_register(part2);
        uint32_t y = get_register(part3);
        uint32_t sum = x + y;
        set_register(part4, sum);
    }

    // sub instruction (rd = rs - rt)
    if ((instruction & 0xfc0007ff) == 0x00000022) {
        uint32_t x = get_register(part2);
        uint32_t y = get_register(part3);
        uint32_t val = x - y;
        set_register(part4, val);
    }

    // mul instruction (rd = rs * rt)
    if ((instruction & 0xfc0007ff) == 0x70000002) {
        uint32_t x = get_register(part2);
        uint32_t y = get_register(part3);
        uint32_t val = x * y;
        set_register(part4, val);
    }

    // and instruction (rd = rs & rt)
    if ((instruction & 0xfc0007ff) == 0x00000024) {
        uint32_t x = get_register(part2);
        uint32_t y = get_register(part3);
        uint32_t val = x & y;
        set_register(part4, val);
    }

    // or instruction (rd = rs | rt)
    if ((instruction & 0xfc0007ff) == 0x00000025) {
        uint32_t x = get_register(part2);
        uint32_t y = get_register(part3);
        uint32_t val = x | y;
        set_register(part4, val);
    }
    
    // xor instruction (rd = rs ^ rt)
    if ((instruction & 0xfc0007ff) == 0x00000026) {
        uint32_t x = get_register(part2);
        uint32_t y = get_register(part3);
        uint32_t val = x ^ y;
        set_register(part4, val);
    }

    // sllv instruction (rd = rs << rt)
    if ((instruction & 0xfc0007ff) == 0x00000004) {
        uint32_t x = get_register(part2);
        uint32_t y = get_register(part3);
        uint32_t val = x << y;
        set_register(part4, val);
    }

    // srlv instruction (rd = rs >> rt)
    if ((instruction & 0xfc0007ff) == 0x00000006) {
        uint32_t x = get_register(part2);
        uint32_t y = get_register(part3);
        uint32_t val = x >> y;
        set_register(part4, val);
    }

    // slt instruction (rd = rs < rt)
    if ((instruction & 0xfc0007ff) == 0x00000002A) {
        uint32_t x = get_register(part2);
        uint32_t y = get_register(part3);
        uint32_t val = x < y;
        set_register(part4, val);
    }

    // Two's complement (print when required)

    // ~x+1=-x

    //int part7_twos_comp = (int16_t) part7;
    //printf("Twos comp: %d\n", part7_twos_comp);

    // addi instruction (rt = rs + I)
    if (part1 == 8) {
        uint32_t x = get_register(part2);
        uint32_t y = part7;
        uint32_t val = x + y;
        set_register(part3, val);
    }


    // andi instruction (rt = rs & I)
    if (part1 == 12) {
        uint32_t x = get_register(part2);
        uint32_t y = part7;
        uint32_t val = x & y;
        set_register(part3, val);
    }

    // ori instruction (rt = rs | I)
    if (part1 == 13) {
        uint32_t x = get_register(part2);
        uint32_t y = part7;
        uint32_t val = x | y;
        set_register(part3, val);
    }

    // xori instruction (rt = rs ^ I)
    if (part1 == 14) {
        uint32_t x = get_register(part2);
        uint32_t y = part7;
        uint32_t val = x ^ y;
        set_register(part3, val);
    }

    // sll instruction (rd = rt << I)
    if (part1 == 0 && part2 == 0 && part6 == 0) {
        uint32_t x = get_register(part3);
        uint32_t y = part5;
        uint32_t val = x << y;
        set_register(part4, val);
    }

    // srl instruction (rd = rt >> I)
    if (part1 == 0 && part2 == 0 && part6 == 2) {
        uint32_t x = get_register(part3);
        uint32_t y = part5;
        uint32_t val = x >> y;
        set_register(part4, val);
    }

    // slti instruction (rt = rs < I)
    if (part1 == 10) {
        uint32_t x = get_register(part2);
        uint32_t y = part7;
        uint32_t val = x < y;
        set_register(part3, val);
    }

    // lui instruction (rt = I << 16)
    if (part1 == 15 && part2 == 0) {
        uint32_t y = part7;
        uint32_t val = y << 16;
        set_register(part3, val);
    }

    // lb instruction (rt = *(int8*)(b + O))
    if (part1 == 32) {
        uint32_t x = get_register(part2) + 8;
        uint32_t val = get_byte(x);
        set_register(part3, val);
    }

    // lh instruction (rt = *(int16*)(b + O))
    if (part1 == 33) {
        uint32_t x = get_register(part2) + 16;
        uint32_t val = get_byte(x);
        set_register(part3, val);
    }

    // lw instruction (rt = *(int32*)(b + O))
    if (part1 == 35) {
        uint32_t x = get_register(part2) + 32;
        uint32_t val = get_byte(x);
        set_register(part3, val);
    }

    // sb instruction (*(uint8*)(b + O) = (rt & 0xff)
    if (part1 == 40) {
        printf("sb $%d, %d($%d)", part3, (int16_t) part7, part2);
    }

    // sh instruction (*(uint16*)(b + O) = (rt & 0xffff))
    if (part1 == 41) {
        printf("sh $%d, %d($%d)", part3, (int16_t) part7, part2);
    }

    // sw instruction (*(uint32*)(b + O) = rt)
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
    // Update program_counter to address of next instructions
    // Most instructions will simply do the following:
    (*program_counter) += 4;
    // Jump & Branch instructions will do something different


    // 0 should be returned, unless an exit syscall is executed
    return 0;
}
