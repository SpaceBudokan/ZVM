#include <malloc.h>
#include <stdio.h>
#include <stdint.h>

#define INT uint64_t
/*The above define sets the size of the bytecode*/
INT *ramArray;
INT ramSize = 100;
INT pc;
INT sp;
INT fp;

void push(INT value)
{
  if(sp == ramSize - 1){
    ramSize = ramSize + 100;
    ramArray = realloc(ramArray, ramSize);
    if(ramArray == NULL){
      printf("ERROR: Unable to allocate ramArray!\n");
      exit 0;
    }
  }
  ramArray[sp] = value;
  sp++;
  return;
}

void pop(void){
  sp--;
}
    
int main(void)
{
  ramArray = malloc(sizeof(INT) * ramSize);
  if(ramArray == NULL){
    printf("ERROR: Unable to allocate ramArray!\n");
  }
  int size = sizeof(ramArray);
  printf("Size: %i\n", size);
  free(ramArray);
  return 0;
}

