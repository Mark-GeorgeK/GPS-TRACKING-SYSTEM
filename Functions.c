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

void SegmentsDisplay(uint8_t firstTwoDigits, uint8_t thirdDigit) {
	GPIO_PORTB_DIR_R &= 0;					//Clearing the previous digit displayed
	GPIO_PORTB_DIR_R |= firstTwoDigits;		//Adding the first two digits on port_B

	GPIO_PORTA_DIR_R &= 3;					//Clearing all bits except UART related bits
	thirdDigit = thirdDigit << 4;			//Shifting the third digit to write on bits 7-6-5-4
	GPIO_PORTA_DIR_R |= thirdDigit;			
	
}


