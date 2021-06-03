#include "Functions.h"
void delay_1sec(void)
 {
	 unsigned long i;
    for( i = 0; i <= 3000000; i++ )
        {}
 }

int main() {
	//Testing SegmentsDisplay , Expected to find (0x56 on Port B, 0x40 on Port A)
	int distance = 95;
	Init();
	
	while(!DestinationReached(distance)){
		distance++;
		SegmentsDisplay(distance);
	  LED_ON(distance);
		delay_1sec();
	}
	while(1){}
}
