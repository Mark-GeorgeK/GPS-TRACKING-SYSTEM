#include "Functions.h"

//Global Variables

// float lon[TURNING_INDIC];
// float lat[TURNING_INDIC];
// float COG[TURNING_INDIC]; 
// float total_distance =0;
// float startingPoint[3];
// float currentCog =0;
// float cogRunningSum =0;
// float cogRunningNum =0;
// int initialCounter = TURNING_INDIC;




//Prototypes


void UART_INIT(void)
{


	SYSCTL_RCGCUART_R|=0x04;        //Activate UART6 clock PD4-->Rx   PD5-->Tx
	SYSCTL_RCGCGPIO_R|=0x08;   	// Activate PortD clock
    	while(!(SYSCTL_PRGPIO_R &0x08)){}
	GPIO_PORTD_LOCK_R=0x4C4F434B; //Port D should be unlocked for PD7
	GPIO_PORTD_CR_R=0xFF;
	UART2_CTL_R &=~0x00000001; 	// Disable UART to start Configuration
    
    // configuring Baud Rate Divisor
    UART2_IBRD_R=104;
    UART2_FBRD_R=11;
    // Writing on LCRH to activate changes for BDR
    UART2_LCRH_R=0x00000070;         //8 bits data, 1 stop bit, NO parity bits,FIFO enabled
    UART2_CTL_R=0x00000301;         //enable UART after Configuration
    GPIO_PORTD_AFSEL_R|=0xC0;       //Alternate Function enabled
    GPIO_PORTD_DEN_R|=0xC0;         //Digital Enabled
    GPIO_PORTD_PCTL_R|= (GPIO_PORTD_PCTL_R&0x00FFFFFF)+0x11000000;
    GPIO_PORTD_AMSEL_R&=~0xC0;      //Disable analog
}
char UART6_Receive(void)
{
    while((UART2_FR_R&0x0010)!=0){}
		return (char)(UART2_DR_R&0xFF);
}
void Init() {
	
	//ACTIVATING CLOCK FOR THE REQUIRED PORTS A,B AND F
	// SYSCTL_RCGCGPIO_R = 0x23;
	// while (!(SYSCTL_PRGPIO_R & 0x23));
    
    //ACTIVATING CLOCK FOR THE REQUIRED PORTS F
    SYSCTL_RCGCGPIO_R = 0x20;
    while (!(SYSCTL_PRGPIO_R & 0x20));

	//UNLOCKING PORTS
	// GPIO_PORTA_LOCK_R = 0X4C4F434B;
	// GPIO_PORTB_LOCK_R = 0X4C4F434B; 
	GPIO_PORTF_LOCK_R = 0X4C4F434B;
	// GPIO_PORTA_CR_R = 0xFF;
	// GPIO_PORTB_CR_R = 0xFF;
	GPIO_PORTF_CR_R = 0x1F;
	
	//AMSEL
	// GPIO_PORTA_AMSEL_R = 0;
	// GPIO_PORTB_AMSEL_R = 0;
	GPIO_PORTF_AMSEL_R = 0;

	//AFSEL
	// GPIO_PORTA_AFSEL_R = 0x3;
	// GPIO_PORTB_AFSEL_R = 0;
	GPIO_PORTF_AFSEL_R = 0;
	
	//PCTL
	// GPIO_PORTA_PCTL_R = 0x11;
	// GPIO_PORTB_PCTL_R = 0;
	GPIO_PORTF_PCTL_R = 0;

	//DEN
	// GPIO_PORTA_DEN_R = 0xFF;
	// GPIO_PORTB_DEN_R = 0xFF;
	GPIO_PORTF_DEN_R = 0x1F;

	//DIR
	// GPIO_PORTA_DIR_R = 0xFC;
	// GPIO_PORTB_DIR_R = 0xFF;
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
float DistanceBetween2Points(float Latitude1,float Longitude1,float Latitude2,float Longitude2) {
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
	return Distance;
}


//Total Distance Functions 
//tested
void ShiftInsert(float arr[], float input){
    int i;
    for(i = 1; i <TURNING_INDIC;i++){
        arr[i-1] = arr[i];
    }
    arr[TURNING_INDIC-1] = input;
}


//tested
float Average(float arr[]){
    float temp =0;
    int i;
    for(i =0; i<TURNING_INDIC;i++){
        temp += arr[i];
    }
    temp = temp/TURNING_INDIC;
    return temp;
}
//tested
bool Turned(float arr[],float currentCog){
    if(fabs(Average(arr) - currentCog) > 80){  //75 could be modified
        return true;
    }
    return false;
}


bool DegCheck(float arr[]){ //could be implemented with average and stuff
    int i;
    for(i = 1;i<TURNING_INDIC-1;i++){
        if( fabs(arr[i] -arr[i-1]) > 40 )   //tweakable
            return false;
    }
    return true;
}

bool Outlier(float currentCog, float inputCog){
    if(fabs(currentCog - inputCog) > 130)
        return true;
    return false;
}

//Destination Reached Condition
bool DestinationReached()	//MODIFIED 
{ if (GPIO_PORTF_DATA_R & 0x10) //if switch 2 is pressed
    return true;
    else
    return false;
//checks distance state, not needed but a place holder for any further modifications and for readability purposes
}

void LED_ON()
{

    if(DestinationReached==true)
        
        GPIO_PORTF_DATA_R |= 0x08; //green
    else
    GPIO_PORTF_DATA_R &=~(0x08);
    //turns on green LED if switch 2 is pressed
}

void delay_1sec(void) 
{
	 unsigned long i;
    for( i = 0; i <= 3000000; i++ )
        {}
}

//reading data from GPS
// getting GPRMC sentence to get latitude and longitude and course over land
bool GPSread(void){
    bool GPScheck = false;
    bool GPRMCflag= true;
    
    int i,j;
    char* str;
    char* GPRMC_ = "$GPRMC";
    char c;
    int term =0;
    while(!GPScheck){
        //getting NMEA sentences
				for(i= 0; i < NMEA_MAX_LEN; i++){
						c= UART6_Receive();
						str[i] = c;
						if(c == '\n' || c == '\r') break;
				}
				
        //checking if NMEA sentence is GPRMC
        for(i= 0; i < 6; i++){
            if(str[i] != GPRMC_[i]){
                GPRMCflag = false;
                break;
            }
        }
        GPScheck = GPRMCflag;
    
        //storing (raw) latitude, longitude, course over ground data in arrays
        if(GPRMCflag){
            for(i = 0; i <strlen(str); i++){
                if(str[i]== ','){
                    term++;
                }
                if(term == 2){
                    if(str[i] =='A'){
                        fix = true;
                    }
                }
                if(term == 3){
                    for( j = 1; j<13; j++){
                        rawLatitude[j] = str[i];
                        i++;
                    }
                }
                if(term == 4){
                    if(str[i] == 'N')
                    rawLatitude[0] = '0';
                    else
                    rawLatitude[0] = '-';
                
                }
                if(term == 5){
                    for( j = 1; j<13; j++){
                        rawLongitude[j] = str[i];
                        i++;
                    }
                }
                if(term == 6){
                    if(str[i] == 'E')
                    rawLongitude[0] = '0';
                    else
                    rawLongitude[0] = '-';
                }
                if(term == 8){
                    for(j=0; j<5; j++){
                        rawCourse[j] = str[i];
                        i++;
                    }
                }

            }
        }
    
    
    }
    return GPScheck;
}
//lat and long from char* to float (degrees)
float parse_rawDegree(char* term) {
  float val = (float)atof(term)/100;
  int16_t dec = (int16_t)val;
  val -= dec;
  val  = val * 100/60 + dec;
  return val;
}
float Latitude() {
  return parse_rawDegree(rawLatitude);
}
float Longitude() {
  return parse_rawDegree(rawLongitude);
}
float CourseLand(){
	return (float)atof(rawCourse);
}

// void SystemInit(){}

unsigned char* TO_ASCII(int n,unsigned char* arr) {
    arr[2] = n % 10 + 48;
    n /= 10;
    arr[1] = n % 10 + 48;
    n /= 10;
    arr[0] = n + 48;
    return arr;
}

//INITIALIZING PORTS
void LCD_INIT(void) {
    SYSCTL_RCGCGPIO_R |= 0x02 ; // Initialize clocks for ports A and B
    while( (SYSCTL_PRGPIO_R & 0x02) == 0 ) {}
    //volatile unsigned long delay;
    //SYSCTL_RCGC2_R |= 0X00000002;   // allow the clock for portB
    //delay = SYSCTL_RCGC2_R;     // short delay for clock
    GPIO_PORTB_AFSEL_R &= ~0xFF;
    GPIO_PORTB_AMSEL_R &= ~0XFF;
    GPIO_PORTB_PCTL_R &= ~0XFF;
    GPIO_PORTB_DIR_R  |= 0XFF;      //set the direction of PB0-7 as output
    GPIO_PORTB_DEN_R  |= 0XFF;


    SYSCTL_RCGCGPIO_R |= 0x01 ; // Initialize clocks for ports A and B
    while( (SYSCTL_PRGPIO_R & 0x01) == 0 ) {}
    //SYSCTL_RCGC2_R |= 0X00000001;   // allow the clock for PA5,6,7
    //delay = SYSCTL_RCGC2_R;     // short delay for clock
    GPIO_PORTA_AFSEL_R &= ~0xE0;    //disable alternative functions for PA5,6,7
    GPIO_PORTA_AMSEL_R &= ~0XE0;    //disable analog function for PA5,6,7
    GPIO_PORTA_PCTL_R &= ~0XE0;     //regular digital pins
    GPIO_PORTA_DIR_R |= 0XE0;       //set the direction of PA5,6,7 as output
    GPIO_PORTA_DEN_R |= 0XE0;       //enable digital PA5,6,7
}

void PRINT_DISTANCE(int distance){
    unsigned char arr[3] = {0,0,0};
    Cursor_pos(0,10);
    TO_ASCII(distance, arr);
    LCD_display(arr);
    msdelay(250);
}

void Cursor_pos(unsigned char x_pos, unsigned char y_pos){
    uint8_t Address =0;
    if (x_pos ==0)
        Address = 0x80;
    else if (x_pos ==1)
        Address = 0xC0;
    if( y_pos <16)
        Address += y_pos;
    LCD_CMD(Address); //check
}


/* delay in milliseconds */
void msdelay(int n) {
     int i,j;
     for(i=0;i<n;i++)
     for(j=0;j<3180;j++){}
}

/* delay in microseconds */
void microdelay(int n) {
     int i,j;
     for(i=0;i<n;i++)
     for(j=0;j<3;j++){}
}

//LCD COMMAND
void LCD_CMD (unsigned char cmd) {
  LCD_RS = 0x00;  //set PA7 register select pin to command
  LCD_RW = 0x00;  //set PA5 r/w pin to write
  GPIO_PORTB_DATA_R = cmd;    //set PB7-0 as the passed command to the function

  LCD_EN = 0x40;  //E pin to 1
  msdelay(50);
  LCD_EN = 0x00;  //E pin to 0
}

//LCD write data
void LCD_WRITE_DATA (unsigned char data) {
    LCD_RS = 0x80;  //set PA7 to data
    LCD_RW = 0x00;  //set pA5 to
    GPIO_PORTB_DATA_R = data;   //write data to PB7-0
    LCD_EN = 0x40;  //E pin to 1
    msdelay(50);
    LCD_EN = 0x00;  //E pin to 0
}

void LCD_display(unsigned char *str) {
    int i;
    for(i = 0; str[i] != '\0'; i++) {
        LCD_WRITE_DATA(str[i]);
        msdelay(50);
    }
}
