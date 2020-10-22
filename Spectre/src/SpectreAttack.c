#include <emmintrin.h>
#include <x86intrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

unsigned int buffer_size = 10;
uint8_t buffer[10] = {0,1,2,3,4,5,6,7,8,9}; 
uint8_t temp = 0;
char *secret = "Some Secret Value";   
uint8_t array[256*4096];

#define CACHE_HIT_THRESHOLD (80)
#define DELTA 1024

// Sandbox Function
uint8_t restrictedAccess(size_t x)
{
  if (x < buffer_size) {
     return buffer[x];
  } else {
     return 0;
  } 
}

void flushSideChannel()
{
  // TODO
  // Add Code here
  //
}

void reloadSideChannel()
{
  // TODO
  // Add Code here
  // Calculate the secret value i
  //
  printf("The Secret = %d.\n",i);
}

void spectreAttack(size_t larger_x)
{
  // TODO
  // Add Code here
  //

  // Ask restrictedAccess() to return 
  // the secret in out-of-order execution. 
  s = restrictedAccess(larger_x);  
  array[s*4096 + DELTA] += 88;  
}

int main() {
  flushSideChannel();
  
  // Calculate offset of secret and apply
  // spectre attack
  size_t larger_x = (size_t)(secret - (char*)buffer);  
  spectreAttack(larger_x);
  
  reloadSideChannel();
  
  return (0);
}
