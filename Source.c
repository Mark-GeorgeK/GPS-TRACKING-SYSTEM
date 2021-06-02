#include "Functions.h"

int main() {
	//Testing SegmentsDisplay , Expected to find (0x56 on Port B, 0x40 on Port A)
	int distance = 456;
	Init();
	SegmentsDisplay(distance);
	
	return 0;
}
