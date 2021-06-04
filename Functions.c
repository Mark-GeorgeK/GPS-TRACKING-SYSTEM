#include "Functions.h"

void Init() {
	
	//ACTIVATING CLOCK FOR THE REQUIRED PORTS A,B AND F
	SYSCTL_RCGCGPIO_R = 0x23;
	while (!(SYSCTL_PRGPIO_R & 0x23));

	//UNLOCKING PORTS
	GPIO_PORTA_LOCK_R = 0X4C4F434B;
	GPIO_PORTB_LOCK_R = 0X4C4F434B; 
	GPIO_PORTF_LOCK_R = 0X4C4F434B;
	GPIO_PORTA_CR_R = 0xFF;
	GPIO_PORTB_CR_R = 0xFF;
	GPIO_PORTF_CR_R = 0x1F;
	
	//AMSEL
	GPIO_PORTA_AMSEL_R = 0;
	GPIO_PORTB_AMSEL_R = 0;
	GPIO_PORTF_AMSEL_R = 0;

	//AFSEL
	GPIO_PORTA_AFSEL_R = 0x3;
	GPIO_PORTB_AFSEL_R = 0;
	GPIO_PORTF_AFSEL_R = 0;
	
	//PCTL
	GPIO_PORTA_PCTL_R = 0x11;
	GPIO_PORTB_PCTL_R = 0;
	GPIO_PORTF_PCTL_R = 0;

	//DEN
	GPIO_PORTA_DEN_R = 0xFF;
	GPIO_PORTB_DEN_R = 0xFF;
	GPIO_PORTF_DEN_R = 0x1F;

	//DIR
	GPIO_PORTA_DIR_R = 0xFC;
	GPIO_PORTB_DIR_R = 0xFF;
	GPIO_PORTF_DIR_R = 0xE;
}

//functions for 7 segment display 
//if number is 348, firstDigit is 8, secondDigit is 4, thirdDigit is 3 
uint8_t FirstTwoDigits(int distance){
    uint8_t firstTwoDigits;
    uint8_t firstDigit = (uint8_t)(distance % 10);
    uint8_t secondDigit = (uint8_t)((distance/10) %10);
    firstDigit &= 0x0F;
    firstTwoDigits = (firstDigit)|(secondDigit << 4);
    return firstTwoDigits;
}

uint8_t ThirdDigit(int distance){
    uint8_t thirdDigit = (uint8_t)(distance/100);
    return thirdDigit;
}

void SegmentsDisplay(int distance) {
	uint8_t firstTwo = FirstTwoDigits(distance);
	uint8_t third = ThirdDigit(distance);

	GPIO_PORTB_DATA_R &= 0;					//Clearing the previous digit displayed
	GPIO_PORTB_DATA_R |= firstTwo;		//Adding the first two digits on port_B

	GPIO_PORTA_DATA_R &= 3;					//Clearing all bits except UART related bits
	third = third<< 4;			//Shifting the third digit to write on bits 7-6-5-4
	GPIO_PORTA_DATA_R |= third;			
	
}

//Function to convert from deg 2 rad,our formula requires working on rads
float Deg2Rad(float Deg) {
    return (Deg*(M_PI/180));
}

//Harvesine Formula for calculating distance between two points
int DistanceBetween2Points(float Latitude1,float Longitude1,float Latitude2,float Longitude2) {
	int EarthDiameter=2*6371000; // Diameter of Earth in Meters
	float Latitude1Rad=Deg2Rad(Latitude1);
	float Latitude2Rad=Deg2Rad(Latitude2);
	float Longitude1Rad=Deg2Rad(Longitude1);
	float Longitude2Rad=Deg2Rad(Longitude2);
	//formula
	float LatitudeDiff=Latitude2Rad-Latitude1Rad;
	float LongitudeDiff=Longitude2Rad-Longitude1Rad;
	float squareroot= sqrtf(pow(sin(LatitudeDiff/ 2), 2) + cos(Latitude1Rad)*cos(Latitude2Rad)*pow(sin(LongitudeDiff / 2), 2));
	float Distance=EarthDiameter*asinf(squareroot) ;
	return (int)Distance;
}

bool DestinationReached(int distance)
{ if (distance>=100)
    return true;
    else
    return false;
//checks distance state, not needed but a place holder for any further modifications and for readability purposes
}

void LED_ON(int distance)
{

if(DestinationReached(distance))
    
    GPIO_PORTF_DATA_R |= 0x08; //green
else
GPIO_PORTF_DATA_R &=~(0x08);
//turns on green LED if distance >100
}

void delay_1sec(void)
 {
	 unsigned long i;
    for( i = 0; i <= 3000000; i++ )
        {}
 }
