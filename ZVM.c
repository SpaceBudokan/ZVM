#include <malloc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define INT uint64_t
/*The above sets the size of the bytecode words*/
#define FUNCS 
INT programRam[100];
INT *stackArray;
INT stackSize = 100;
INT pc = 0;
INT sp = -1;
INT fp = -1;

void push(INT value)
{
  if(sp == stackSize - 1){
    stackSize = stackSize + 100;
    stackArray = realloc(stackArray, stackSize);
    if(stackArray == NULL){
      printf("ERROR: Unable to allocate stackArray!\n");
      exit(0);
    }
  }
  sp++;
  stackArray[sp] = value;
  return;
}

void pop(void){
  sp--;
  return;
}


void noop(void)
{
  /* do nothing */
  return;
}

void iconst(void)
{
  pc++;
  push(programRam[pc]);
  return;
}

void load(void)
{
  return;
}

void store(void)
{
  return;
}

void add(void)
{
  stackArray[sp - 1] = stackArray[sp] + stackArray[sp - 1];
  pop();
  return;
}
void sub(void)
{
  return;
}

void mult(void)
{
  return;
}

void divide(void)
{
  return;
}

void jump(void)
{
  return;
}

void jsubr(void)
{
  return;
}

void ret(void)
{
  return;
}

void beqz(void)
{
  return;
}

void bneqz(void)
{
  return;
}

void pchar(void)
{
  printf("%c", (char)stackArray[sp]);
  pop();
  return;
}

void pnum(void)
{
  printf("%lu",stackArray[sp]);
  pop();
  return;
}

void decode(void)
{
  /*using a jumptable because some compilers aren't too eager to optimize switch-case loops to jumptables */
  void (*jumptable[15])(void) = {noop, iconst, load, store, add, sub, mult,
				divide, jump, jsubr, ret, beqz, bneqz, pchar, pnum};
  jumptable[programRam[pc]]();
  return;
}


    
int main(int argc, char **argv)
{
  stackArray = malloc(sizeof(INT) * stackSize);
  if(stackArray == NULL){
    printf("ERROR: Unable to allocate stackArray!\n");
    return 1;
  }

  programRam[0] = 1;
  programRam[1] = 1;
  programRam[2] = 1;
  programRam[3] = 1;
  programRam[4] = 4;
  programRam[5] = 14;
  while(pc < 6){
    decode();
    pc++;
  }
  
  free(stackArray);
  return 0;
}

