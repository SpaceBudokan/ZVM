#include <malloc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define INT uint64_t
/*The above sets the size of the bytecode words*/
INT programRam[100];
INT *stackArray;
INT stackSize = 100;
INT pc = 0; /* program counter */
INT sp = -1; /* stack pointer */
INT fp = -1; /* frame pointer */
INT retval; /*value to return from function call */
INT numargs; /*number of arguments on the stack for a subroutine */
INT misc; /* miscellaneous pointer for loading and storing */
INT programLength;

static inline INT ATOI(const char *str)
{
  return (INT)atoll(str);
}

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
  misc = programRam[stackArray[sp]];
  pop();
  push(programRam[misc]);
  return;
}

void store(void)
{
  misc = programRam[stackArray[sp]];
  pop();
  programRam[misc] = stackArray[sp];
  pop();
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
  stackArray[sp - 1] = stackArray[sp - 1] - stackArray[sp];
  pop();
  return;
}

void mult(void)
{
  stackArray[sp - 1] = stackArray[sp - 1] * stackArray[sp];
  pop();
  return;
}

void divide(void)
{
  stackArray[sp - 1] = stackArray[sp - 1] / stackArray[sp];
  pop();
  return;
}

void eq(void)
{
  if(stackArray[sp-1] == stackArray[sp]){
    stackArray[sp - 1] = 1;
  } else{
    stackArray[sp - 1] = 0;
  }
  pop();
  return;
}

void lt(void)
{
  if(stackArray[sp-1] < stackArray[sp]){
    stackArray[sp - 1] = 1;
  } else{
    stackArray[sp - 1] = 0;
  }
  pop();
  return;
}

void gt(void)
{
  if(stackArray[sp-1] > stackArray[sp]){
    stackArray[sp - 1] = 1;
  } else{
    stackArray[sp - 1] = 0;
  }
  pop();
  return;
}

void not(void)
{
  if(stackArray[sp] == 0){
    stackArray[sp] = 1;
  } else{
    stackArray[sp] = 0;
  }
  return;
}

void and(void)
{
  if(stackArray[sp - 1] != 0 && stackArray[sp] != 0){
    stackArray[sp - 1] = 1;
  } else{
    stackArray[sp -1] = 0;
  }
  pop();
  return;
}

void or(void)
{
  if(stackArray[sp - 1] != 0 || stackArray[sp] != 0){
    stackArray[sp - 1] = 1;
  } else{
    stackArray[sp -1] = 0;
  }
  pop();
  return;
}

void xor(void)
{
  if(stackArray[sp - 1] != 0 && stackArray[sp] !=0){
    stackArray[sp - 1] = 0;
  } else if(stackArray[sp - 1] == 0 && stackArray[sp] ==0){
    stackArray[sp -1] = 0;
  } else{
    stackArray[sp -1] = 1;
  }
  pop();
  return;
}

void bnot(void)
{
  stackArray[sp] = ~stackArray[sp];
  return;
}

void band(void)
{
  stackArray[sp -1] = stackArray[sp -1] & stackArray[sp];
  pop();
  return;
}

void bor(void)
{
  stackArray[sp -1] = stackArray[sp -1] | stackArray[sp];
  pop();
  return;
}

void bxor(void)
{
  stackArray[sp -1] = stackArray[sp -1] ^ stackArray[sp];
  pop();
  return;
}

void jump(void)
{
  pc++;
  pc = programRam[pc] - 1;
  return;
}
void dup(void)
{
  push(stackArray[sp]);
  return;
}

void swap()
{
  misc = stackArray[sp];
  stackArray[sp] = stackArray[sp - 1];
  stackArray[sp - 1] = misc;
  return;
}

void over()
{
  push(stackArray[sp - 1]);
  return;
}

void jsubr(void)
{
  INT subaddr;
  pc++;
  subaddr = programRam[pc];
  pc++;
  numargs = programRam[pc];
  push(numargs);
  push(fp);
  push(pc);
  fp = sp;
  pc = subaddr - 1;
  return;
}

void ret(void)
{
  retval= stackArray[sp];
  pop();
  sp = fp;
  pc = stackArray[sp];
  pop();
  fp = stackArray[sp];
  pop();
  numargs = stackArray[sp];
  sp -= numargs;
  stackArray[sp] = retval;
  return;
}

void beqz(void)
{
  pc++;
  if(stackArray[sp] == 0){
    pc = programRam[pc] - 1;
  }
  pop();
  return;
}

void bneqz(void)
{
  pc++;
  if(stackArray[sp] != 0){
    pc = programRam[pc] - 1;
  }
  pop();
  return;
}

void pchar(void)
{
  printf("%c", (char)(stackArray[sp]));
  pop();
  return;
}

void pnum(void)
{
  printf("%lu",stackArray[sp]);
  pop();
  return;
}

void halt(void)
{
  pc = programLength - 1;
  return;
}

void decode(void)
{
  /*using a jumptable because some compilers aren't too eager to optimize switch-case loops to jumptables */
  void (*jumptable[31])(void) = {noop, iconst, load, store, add,
				 sub, mult, divide, eq, lt,
				 gt, not, and, or, xor,
				 bnot, band, bor, bxor, jump,
				 dup, swap, over, jsubr, ret,
				 beqz, bneqz, pchar, pnum, pop,
				 halt};
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
  programRam[2] = 0;
  
  programLength = 3;
  while(pc < programLength){
    decode();
    pc++;
  }
  
  free(stackArray);
  return 0;
}

