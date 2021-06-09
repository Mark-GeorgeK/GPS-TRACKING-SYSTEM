#include "tm4c123gh6pm.h"
#include <inttypes.h>
#include <stdio.h>
#include <math.h> 
#include <stdbool.h>
#include <string.h>

#define M_PI 3.142857
//UART 
void UART_INIT(void);
char UART6_Receive(void);
// Ports
void Init(void);

//for 7 segment display
uint8_t FirstTwoDigits(int distance);
uint8_t ThirdDigit(int distance);
void SegmentsDisplay(int distance);

//for turning on LED
void LED_ON(int distance);
bool DestinationReached(int distance);

//for distance calculating
float Deg2Rad(float Deg);
float DistanceBetween2Points(float Latitude1,float Longitude1,float Latitude2,float Longitude2);

//Total Distance functions
void ShiftInsert(float arr[], float input);
float Average(float arr[]);
bool Turned(float arr[],float currentCog);
bool DegCheck(float arr[]);
bool Outlier(float currentCog, float inputCog);

//Delay function
void delay_1sec(void);

//reading gps data
bool GPSread(void);