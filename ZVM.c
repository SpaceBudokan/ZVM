#include <malloc.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>


#define INT uint64_t
/*The above sets the size of the bytecode words*/
INT *ramArray;
INT ramSize = 100;
INT pc = 0;
INT sp = -1;
INT fp = -1;
void iconst(void)
{
}

void load(void)
{
}

void store(void)
{
}

void add(void)
{
}
  
void decode(void)
{
  void (*jumptable[4])(void) = {iconst, load, store, add};
  jumptable[ramArray[pc]]();
}

void push(INT value)
{
  if(sp == ramSize - 1){
    ramSize = ramSize + 100;
    ramArray = realloc(ramArray, ramSize);
    if(ramArray == NULL){
      printf("ERROR: Unable to allocate ramArray!\n");
      exit(0);
    }
  }
  ramArray[sp] = value;
  sp++;
  return;
}

void pop(void){
  sp--;
}
    
int main(int argc, char **argv)
{
  ramArray = malloc(sizeof(INT) * ramSize);
  if(ramArray == NULL){
    printf("ERROR: Unable to allocate ramArray!\n");
  }

  printf("Size: %i\n", size);

  
  
  free(ramArray);
  return 0;
}

