#include "main.h"



int add_fast(int pc_offset, int c, int a)
{
  /* FIRST_DEGREE
  mem[c] = mem[c] + mem[a]
  mem[a] = 0
  */
  int previous_pc_offset = pc_offset;
  pc_offset = negate_fast(pc_offset,1,c);
  pc_offset = subtract_fast(pc_offset,1,a);
  pc_offset = negate_fast(pc_offset,c,1);
  printf("add_fast: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int add(int pc_offset, int c, int a)
{
  /* THIRD_DEGREE
  mem[c] = mem[c] + mem[a]
  */
  int previous_pc_offset = pc_offset;
  pc_offset = negate_fast(pc_offset,2,c);
  pc_offset = subtract(pc_offset,2,a);
  pc_offset = negate_fast(pc_offset,c,2);
  printf("add: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int decrement(int pc_offset, int a)
{
  /* ZEROTH_DEGREE
    mem[a] = mem[a] - 1
  */
  program1[pc_offset] = a;
  program2[pc_offset] = pc_offset+1;

  return pc_offset+1;
}
void exit_program(int pc_offset)
{
  //FIRST_DEGREE
  program1[pc_offset] = 0;
  program2[pc_offset] = -1;
}
int increment(int pc_offset, int address)
{
  //ZEROTH_DEGREE
  for(int i=0;i<255;i=i+1)
  {
    program1[pc_offset+i] = address;
    program2[pc_offset+i] = pc_offset+i+1;
  }
  return pc_offset+255;
}
void jump(int pc_offset, int address)
{
  for(int i=0;i<256*2;i=i+2)
  {
    program1[pc_offset+i] = address;
    program2[pc_offset+i] = pc_offset+i+2;

    program1[pc_offset+i+1] = 0;
    program2[pc_offset+i+1] = pc_offset - ((i/2)+1);
  }

}
int jump_immediate(int pc_offset, int address_to_jump)
{
  /* FIRST_DEGREE
  PC = a
  */
  program1[pc_offset] = 0;
  program2[pc_offset] = address_to_jump;

  program1[pc_offset+1] = 0;
  program2[pc_offset+1] = address_to_jump;

  return pc_offset+2;
}
int jump_immediate_if_zero(int pc_offset, int pointer_to_value,int address_to_jump)
{
  /* FORTH_DEGREE
  if mem[pointer_to_value] = 0
    pc = address_to_jump
  */
  pc_offset = move (pc_offset,3,pointer_to_value);

  program1[pc_offset] = 3;
  program2[pc_offset] = pc_offset+3; // if not zero

  program1[pc_offset+1] = 0;
  program2[pc_offset+1] = address_to_jump; //if zero
  program1[pc_offset+2] = 0;
  program2[pc_offset+2] = address_to_jump; //if zero

  return pc_offset+3;
}
int jump_immediate_if_not_zero(int pc_offset,int pointer_to_value,int address_to_jump)
{
  /* FORTH_DEGREE
  if mem[pointer_to_value] =! 0
    pc = address_to_jump
  */
  pc_offset = increment(pc_offset,pointer_to_value);
  program1[pc_offset] = pointer_to_value;
  program2[pc_offset] = address_to_jump; // if not zero
  return pc_offset+1;
}
int move_fast(int pc_offset, int c, int a)
{
  /* SECOND_DEGREE
  mem[c] = mem[a]
  mem[a] = 0;
  */
  int previous_pc_offset = pc_offset;
  pc_offset = negate_fast(pc_offset,1,a);
  pc_offset = negate_fast(pc_offset,c,1);
  printf("move_fast: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int move(int pc_offset, int c, int a)
{
  /* THIRD_DEGREE
  mem[c] = mem[a]
  */
  int previous_pc_offset = pc_offset;
  pc_offset = store_immediate(pc_offset,2,0);
  pc_offset = subtract(pc_offset,2,a);
  pc_offset = negate_fast(pc_offset,c,2);
  printf("move: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int move_slow(int pc_offset, int c, int a)
{
  /* THIRD_DEGREE
  mem[c] = mem[a]
  */
  int previous_pc_offset = pc_offset;
  pc_offset = store_immediate(pc_offset,c,0);
  pc_offset = subtract(pc_offset,c,a);
  pc_offset = negate(pc_offset,c);
  printf("move_slow: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int mul(int pc_offset, int A, int B)
{
  /* FIFTH_DEGREE
  mem[A] = mem[A] * mem[B]
  */

  pc_offset = store_immediate(pc_offset,4,0);
  pc_offset = increment(pc_offset,A);

  int outer_loop = pc_offset;
  program1[pc_offset] = A;
  program2[pc_offset] = pc_offset+5;

  program1[pc_offset+1] = 0;
  program2[pc_offset+1] = pc_offset+3; //exit

  program1[pc_offset+2] = 0;
  program2[pc_offset+2] = pc_offset+3; //exit

  int exit_loop = pc_offset+3;

  pc_offset = move(pc_offset+5,3,B);

  for(int i=0 ; i<256*4 ; i=i+4)
  {
    program1[pc_offset+i] = 4;
    program2[pc_offset+i] = pc_offset+i+1;

    program1[pc_offset+i+1] = 3;
    program2[pc_offset+i+1] = pc_offset+i+4;

    program1[pc_offset+i+2] = 0;
    program2[pc_offset+i+2] = outer_loop;

    program1[pc_offset+i+3] = 0;
    program2[pc_offset+i+3] = outer_loop;
  }
  pc_offset = pc_offset + 256*4;

  program1[exit_loop] = 0;
  program2[exit_loop] = pc_offset;

  program1[exit_loop+1] = 0;
  program2[exit_loop+1] = pc_offset;

  pc_offset = negate_fast(pc_offset ,A,4);
  return pc_offset;
}
int negate_fast(int pc_offset, int c, int a)
{
  /* FIRST_DEGREE
  mem[c] = -mem[a]
  mem[a] = 0;
  */
  int previous_pc_offset = pc_offset;
  pc_offset = store_immediate(pc_offset,c,0);
  pc_offset = subtract_fast(pc_offset,c,a);
  printf("negate_fast: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int negate(int pc_offset, int a)
{
  /* THIRD_DEGREE
  mem[c] = -mem[a]
  */
  int previous_pc_offset = pc_offset;
  pc_offset = negate_fast(pc_offset,1,a);
  pc_offset = negate_fast(pc_offset,2,1);
  pc_offset = negate_fast(pc_offset,a,2);
  printf("negate: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int negate_slow(int pc_offset, int a) //mem[c] = -mem[a]
{
  /* THIRD_DEGREE
  mem[c] = -mem[a]
  */
  int previous_pc_offset = pc_offset;
  pc_offset = store_immediate(pc_offset,2,0);
  pc_offset = subtract_fast(pc_offset,2,a);
  pc_offset = move_fast(pc_offset,a,2);
  printf("negate_slow: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int subtract_fast(int pc_offset, int c, int a){
  /* FIRST_DEGREE
  mem[c] = mem[c] - mem[a]
  mem[a] = 0;
  */
  int previous_pc_offset = pc_offset;
  int exit = pc_offset+(4*M);
  for(int i=0;i<=(M-1)*4;i=i+4)
  {
    program1[pc_offset+i] = c;
    program2[pc_offset+i] = pc_offset+i+1;

    program1[pc_offset+i+1] = a;
    program2[pc_offset+i+1] = pc_offset+i+4;

    program1[pc_offset+i+2] = 0;
    program2[pc_offset+i+2] = exit;

    program1[pc_offset+i+3] = 0;
    program2[pc_offset+i+3] = exit;
  }
  printf("subtract_fast: %d\n",exit-previous_pc_offset);
  return exit;
}
int subtract(int pc_offset, int c, int a){
  /* SECOND_DEGREE
  mem[c] = mem[c] - mem[a]
  */
  int previous_pc_offset = pc_offset;

  pc_offset = increment (pc_offset,a);

  program1[pc_offset] = a;
  program2[pc_offset] = pc_offset+3;

  program1[pc_offset+1] = 0;
  program2[pc_offset+1] = pc_offset + 3 + 5*M;

  program1[pc_offset+2] = 0;
  program2[pc_offset+2] = pc_offset + 3 + 5*M;

  pc_offset = pc_offset + 3;

  pc_offset = store_immediate(pc_offset,1,0);

  for(int i=0;i<=(M-1)*5;i=i+5)
  {
    program1[pc_offset+i] = c;
    program2[pc_offset+i] = pc_offset+i+1;

    program1[pc_offset+i+1] = 1;
    program2[pc_offset+i+1] = pc_offset+i+2;

    program1[pc_offset+i+2] = a;
    program2[pc_offset+i+2] = pc_offset+i+5;

    program1[pc_offset+i+3] = 0;
    program2[pc_offset+i+3] = pc_offset+(5*M);

    program1[pc_offset+i+4] = 0;
    program2[pc_offset+i+4] = pc_offset+(5*M);
  }
  pc_offset = pc_offset+(5*M);
  pc_offset = negate_fast(pc_offset,a,1);
  printf("subtract: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int subtract_slow(int pc_offset, int c, int a)
{
  /* SECOND_DEGREE
  mem[c] = mem[c] - mem[a]
  */
  int previous_pc_offset = pc_offset;
  pc_offset = move(pc_offset,1,a);
  pc_offset = subtract_fast(pc_offset,c,1);
  printf("subtract_slow: %d\n",pc_offset-previous_pc_offset);
  return pc_offset;
}
int store_immediate(int pc_offset, int address ,int word)
{
  /* ZEROTH_DEGREE
  mem[address] = word
  */
  for(int i=0;i<512;i=i+2)
  {
    program1[pc_offset+i] = address;
    program2[pc_offset+i] = pc_offset+i+2;

    program1[pc_offset+i+1] = address;
    program2[pc_offset+i+1] = pc_offset+512;
  }
  for(int i=512;i<512+(255-word);i=i+1)
  {
    program1[pc_offset+i] = address;
    program2[pc_offset+i] = pc_offset+i+1;
  }
  return pc_offset+512+(255-word);
}
void print_memory_pc(uint8_t *memory,int pc){
  //print memory
  printf("\nMEMORY\tPC=%d\n",pc);
  for(int i=0;i<MEMORY_SIZE;i++)
  {
    printf("memory[%d] = %u\n",i,memory[i]);
  }
}
void printfcomma (int n) {
    int n2 = 0;
    int scale = 1;
    if (n < 0) {
        printf ("-");
        n = -n;
    }
    while (n >= 1000) {
        n2 = n2 + scale * (n % 1000);
        n /= 1000;
        scale *= 1000;
    }
    printf ("%d", n);
    while (scale != 1) {
        scale /= 1000;
        n = n2 / scale;
        n2 = n2  % scale;
        printf (",%03d", n);
    }
}
int debug_dump_memory(int pc_offset){
  program1[pc_offset] = -1;
  return pc_offset + 1;
}

int main(int argc, char **argv){

  memory = (uint8_t*) calloc(MEMORY_SIZE, sizeof(int));

  int link = 0;
  link = store_immediate(link,9,100);
  link = negate_slow(link,9);
  link = negate_slow(link,9);
  link = debug_dump_memory(link);
  exit_program(link);

/*
// ******** PROGRAM *************
  int link = 0;
  link = store_immediate(link,9,100);
  link = store_immediate(link,8,4);
  link = debug_dump_memory(link);
  int label_1 = link;
  link = negate_slow(link,9);
  link = decrement(link,8);
  //link = debug_dump_memory(link);
  link = jump_immediate_if_not_zero(link,8,label_1);
  link = debug_dump_memory(link);
  exit_program(link);
// ******************************
*/


#ifdef COUNT_EXECUTED_INSTRUCTIONS
int executed_instruction_count = 0;
#endif

  int pc = 0;
  while ( 0<=pc && pc<PROGRAM_SIZE )
  {
    if( 0 <= program1[pc] && program1[pc] < MEMORY_SIZE )
    {
      if(--memory[program1[pc]] == 0)
        pc++;
      else
        pc = program2[pc];
    }
    else
    {
      print_memory_pc(memory,pc);
      pc++;
    }

      #ifdef COUNT_EXECUTED_INSTRUCTIONS
      executed_instruction_count++;
      #endif
  }
  //printf("\nLAST PC = %d\n",link);
  #ifdef COUNT_EXECUTED_INSTRUCTIONS
  printf("executed_instruction_count: ");
  printfcomma(executed_instruction_count);
  printf("\n");
  #endif
  return 0;
}
