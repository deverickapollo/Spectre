#include <emmintrin.h>
#include <x86intrin.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <inttypes.h>


int main(int argc, const char **argv) {

	/*
	 * Clock Cycle of Caching
	 * Note here the caching of array[0] 
	 * occurs due to the caching of adjacent memory.
	 */
    uint8_t array[10*4096];
    for(int j=0;j<10;j++){
  	    uint64_t results[10];
	    int junk=0;
  	    register uint64_t time1, time2;
  		volatile uint8_t *addr;
  		int i;
  		// Initialize the array
  		for(i=0; i<10; i++) array[i*4096]=1;

  		// FLUSH the array from the CPU cache
  		for(i=0; i<10; i++){
			 _mm_clflush(&array[i*4096]);
		}  

		// Access some of the array items
	    array[3*4096] = 100;
	    array[7*4096] = 200;

	    // Clock Cycle Check
	    for(i=0; i<10; i++) {
	    	addr = &array[i*4096];
	    	time1 = __rdtscp(&junk);   junk = *addr;
	    	time2 = __rdtscp(&junk) - time1;  
	    	results[i]=(int)time2;
            printf("Access time for array[%d*4096]: %d CPU cycles\n",i, (int)time2);
	  	}
        bool status=true;
		for(int n=0;n<10;n++){
	    	if(results[n]<results[3] && n != 7){
	        	status=false;
				printf("Element %d was retrieved from memory faster than cached element %d \n",n,3);
	    	}
		   	if(results[n]<results[7] && n != 3){
		        status=false;
				printf("Element %d was retrieved from memory faster than cached elements %d \n",n,7);
		    }
		}
		if(status){
	    	printf("Cache is returning values faster than main memory\n");
		}
   }
	  return 0; 
}
