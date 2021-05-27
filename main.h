#ifndef MAIN_H
#define MAIN_H

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define MEMORY_SIZE 10
#define PROGRAM_SIZE 100000
#define N 8
#define M 256

#define COUNT_EXECUTED_INSTRUCTIONS

#define ZEROTH_DEGREE 0
#define FIRST_DEGREE 1
#define SECOND_DEGREE 2
#define THIRD_DEGREE 3
#define FORTH_DEGREE 4
#define FIFTH_DEGREE 5

//MEMORY ALLOCATION
uint8_t *memory;
//instruction memory
int program1[PROGRAM_SIZE];
int program2[PROGRAM_SIZE];

//FUNCTIONS
int add_fast(int pc_offset, int c, int a);
int add(int pc_offset, int c, int a);
int decrement(int pc_offset, int a);
void exit_program(int pc_offset);
int increment(int pc_offset, int address);
void jump(int pc_offset, int address);
int move_fast(int pc_offset, int c, int a);
int move(int pc_offset, int c, int a);
int move_slow(int pc_offset, int c, int a);
int mul(int pc_offset, int A, int B);
int negate_fast(int pc_offset, int c, int a);
int negate(int pc_offset, int a);
int negate_slow(int pc_offset, int a);
int subtract_fast(int pc_offset, int c, int a);
int subtract(int pc_offset, int c, int a);
int subtract_slow(int pc_offset, int c, int a);
int store_word(int pc_offset, int address ,int word);
int store_immediate(int pc_offset, int address ,int word);

void dump_memory(int8_t *memory);
int main(int argc, char **argv);


#endif /* MAIN_H */
