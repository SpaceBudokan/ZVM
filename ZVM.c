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
  /*takes a value and pushes it onto the stack*/
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
  /*decrements the stack pointer*/
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
  /*pushes an integer constant onto the stack*/
  pc++;
  push(programRam[pc]);
  return;
}

void gload(void)
{
  /*takes the top of stack as the location in program memory to load*/
  misc = programRam[stackArray[sp]];
  pop();
  push(programRam[misc]);
  return;
}

void gstore(void)
{
  /*take the top of stack as the program memory location to load the next value on the stack to*/
  misc = programRam[stackArray[sp]];
  pop();
  programRam[misc] = stackArray[sp];
  pop();
  return;
}

void aload(void) 
{
  /*loads the function argument specified in next word to top of stack*/
  /*indexed from zero starting beneath stack frame, moving down the stack*/
  pc++;
  push(stackArray[(fp - 3) - programRam[pc]]);
  return;
}

void astore(void)
{
  /*stores top of stack to function argument specified in next word*/
  /*indexed from zero starting beneath stack frame, growing down the stack*/
  pc++;
  stackArray[(fp - 3) - programRam[pc]] = stackArray[sp];
  pop();
  return;
}

void lload(void)
{
  /*locals are pushed onto the stack on top of the stack frame.*/
  /*indexed from zero and grow upwards.*/
  pc++;
  push(stackArray[fp + 1 + programRam[pc]]);
  return;
}

void lstore(void)
{
  pc++;
  stackArray[fp + 1 + programRam[pc]] = stackArray[sp];
  pop();
  return;
}

void inc(void)
{
  /*increments the top of stack*/
  stackArray[sp]++;
  return;
}

void dec(void)
{
  /*decrements the top of stack*/
  stackArray[sp]--;
  return;
}

void add(void)
{
  /*adds the top two elements of the stack*/
  stackArray[sp - 1] = stackArray[sp] + stackArray[sp - 1];
  pop();
  return;
}

void ginc(void)
{
  /*takes address from top of stack and increments value at that adress*/
  programRam[stackArray[sp]]++;
  pop();
  return;
}

void gdec(void)
{
  /*takes address from top of stack and decrements it*/
  programRam[stackArray[sp]]--;
  pop();
  return;
}

void ainc(void)
{
  /*increments argument at offset in next word*/
  pc++;
  stackArray[(fp - 3) - strackArray[sp]]++;
  pop();
  return;
}

void adec(void)
{
  /*decrements argument at offset in next word*/
  pc++;
  stackArray[(fp - 3) - strackArray[sp]]--;
  pop();
  return;
}

void linc(void)
{
  /*increments local variable at offset in next word*/
  pc++;
  stackArray[fp + 1 + programRam[pc]]++;
  return;
}

void ldec(void)
{
  /*decrements local variable at offset in next word*/
  pc++;
  stackArray[fp + 1 + programRam[pc]]--;
  return;
}


void sub(void)
{
  /*subtracts the top two elements of the stack*/
  stackArray[sp - 1] = stackArray[sp - 1] - stackArray[sp];
  pop();
  return;
}

void mult(void)
{
  /*multiplies the top two elements of the stack*/
  stackArray[sp - 1] = stackArray[sp - 1] * stackArray[sp];
  pop();
  return;
}

void divide(void)
{
  /*divides the second element of the stack by the first*/
  stackArray[sp - 1] = stackArray[sp - 1] / stackArray[sp];
  pop();
  return;
}

void gaddeq(void)
{
  return;
}

void gsubeq(void)
{
  return;
}

void gmuleq(void)
{
  return;
}

void gdiveq(void)
{
  return;
}

void aaddeq(void)
{
  return;
}

void asubeq(void)
{
  return;
}

void amuleq(void)
{
  return;
}

void adiveq(void)
{
  return;
}

void laddeq(void)
{
  return;
}

void lsubeq(void)
{
  return;
}

void lmuleq(void)
{
  return;
}

void ldiveq(void)
{
  return;
}

void eq(void)
{
  /*tests the top two elements of stack to see if they are equal. leaves 0 if false, 1 if true*/
  if(stackArray[sp - 1] == stackArray[sp]){
    stackArray[sp - 1] = 1;
  } else{
    stackArray[sp - 1] = 0;
  }
  pop();
  return;
}

void lt(void)
{
  /*tests if the second element of the stack is less than the top. leaves 0 if false, 1 if true*/
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
  /*tests if the second element of the stack is greater than the top. leaves 0 if false, 1 if true*/
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
   /*logical not. leaves 1 if top of stack is 0, otherwise leaves 0*/
  if(stackArray[sp] == 0){
    stackArray[sp] = 1;
  } else{
    stackArray[sp] = 0;
  }
  return;
}

void and(void)
{
  /*logical and. leaves 1 if top two elements of the stack are both 0 or both nonzero. otherwise leaves 0*/
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
  /*logical or. leaves 1 if at least one of the top two elements of the stack are nonzero. otherwise leaves 0*/
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
  /*logical exclusive or. leaves 1 if one and only oneof the top two elements of the stack is 0. otherwise leaves 0*/
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
  /*bitwise not*/
  stackArray[sp] = ~stackArray[sp];
  return;
}

void band(void)
{
  /*bitwise and*/
  stackArray[sp -1] = stackArray[sp -1] & stackArray[sp];
  pop();
  return;
}

void bor(void)
{
  /*bitwise or*/
  stackArray[sp -1] = stackArray[sp -1] | stackArray[sp];
  pop();
  return;
}

void bxor(void)
{
  /*bitwise exclusive or*/
  stackArray[sp -1] = stackArray[sp -1] ^ stackArray[sp];
  pop();
  return;
}

void jump(void)
{
  /*jumps to the location of the next value in program memory*/
  pc++;
  pc = programRam[pc] - 1;
  return;
}
void dup(void)
{
  /*duplicates the top of stack*/
  push(stackArray[sp]);
  return;
}

void swap()
{
  /*changes the location of the top two elements of the stack*/
  misc = stackArray[sp];
  stackArray[sp] = stackArray[sp - 1];
  stackArray[sp - 1] = misc;
  return;
}

void over()
{
  /*copies the second element of the stack*/
  push(stackArray[sp - 1]);
  return;
}

void jsubr(void)
{
  /*jumps to a subroutine. expects arguments to already be on the stack*/
  /*next word is location to jump to*/
  /*second word is number of arguments subroutine expects*/
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
  /*returns from a subroutine*/
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
  /*jumps to a memory location if the top of stack is equal to 0*/
  pc++;
  if(stackArray[sp] == 0){
    pc = programRam[pc] - 1;
  }
  pop();
  return;
}

void bneqz(void)
{
  /*jumps to a memory location if top of stack is nonzero*/
  pc++;
  if(stackArray[sp] != 0){
    pc = programRam[pc] - 1;
  }
  pop();
  return;
}

void pchar(void)
{
  /*prints the ascii character of value on top of stack*/
  printf("%c", (char)(stackArray[sp]));
  pop();
  return;
}

void pnum(void)
{
  /*prints the value on the top of stack as an integer*/
  printf("%lu",stackArray[sp]);
  pop();
  return;
}

void halt(void)
{
  /*imediately jumps to end of program*/
  pc = programLength - 1;
  return;
}

void decode(void)
{
  /*using a jumptable because some compilers aren't too eager to optimize switch-case loops to jumptables */
  void (*jumptable[55])(void) = {noop, iconst, gload, gstore, aload,
				 astore, lload, lstore, inc, dec,
				 ginc, gdec, ainc, adec, linc,
				 ldec,  add, sub, mult, divide,
				 gaddeq, gsubeq, gmuleq, gdiveq, aaddeq,
				 asubeq, amuleq, adiveq, laddeq, lsubeq,
				 lmuleq, ldiveq, eq, lt, gt,
				 not, and, or, xor, bnot,
				 band, bor, bxor, jump, dup,
				 swap, over, jsubr, ret, beqz,
				 bneqz, pchar,pnum, pop, halt};
  
  jumptable[programRam[pc]]();
  return;
}


    
int main(int argc, char **argv)
{
  int i = 0;
  stackArray = malloc(sizeof(INT) * stackSize);
  if(stackArray == NULL){
    printf("ERROR: Unable to allocate stackArray!\n");
    return 1;
  }


  programRam[0] = 1;
  programRam[1] = 99;
  programRam[2] = 1;
  programRam[3] = 999;
  programRam[4] = 0;
  programRam[5] = 47;
  programRam[6] = 9;
  programRam[7] = 2;
  programRam[8] = 54;
  programRam[9] = 1;
  programRam[10] = 3;
  programRam[11] = 1;
  programRam[12] = 5;
  programRam[13] = 7;
  programRam[14] = 0;
  programRam[15] = 52;
  programRam[16] = 1;
  programRam[17] = 0;
  programRam[18] = 48;
  
  programLength = 19;
  
  while(pc < programLength){
    decode();
    pc++;
    i = 0;
    while(i <= sp){
      printf("%i ", (int)stackArray[i]);
      i++;
    }
    printf("\n");
  }
  
  free(stackArray);
  return 0;
}

