#include "tm4c123gh6pm.h"
#include <inttypes.h>
#include <stdio.h>
#include <math.h> 

void Init(void);

//for 7 segment display
uint8_t FirstTwoDigits(int distance);
uint8_t ThirdDigit(int distance);
void SegmentsDisplay(int distance);

//for turning on LED
void LED_ON(int distance);
bool DestinationReached(int distance);