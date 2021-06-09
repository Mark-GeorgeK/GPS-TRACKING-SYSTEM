#include "tm4c123gh6pm.h"
#include <inttypes.h>
#include <stdio.h>
#include <math.h> 
#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#define LCD_RS  (*((volatile unsigned long *)0x40004200))    //PA.7 for register select pin
#define LCD_EN  (*((volatile unsigned long *)0x40004100))   //PA.6 for enable pin
#define LCD_RW  (*((volatile unsigned long *)0x40004080))   //PA.5 for rw pin

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

//lat and long from char* to float (degrees)
float parse_rawDegree(char* term);
float Latitude(void);
float Longitude(void);
float CourseLand(void);

void msdelay(int n);
void microdelay(int n);
void LCD_INIT(void);
void LCD_CMD (unsigned char cmd);
void LCD_WRITE_DATA (unsigned char data);
void LCD_display(unsigned char *str);
void Cursor_pos(unsigned char x_pos, unsigned char y_pos);
void PRINT_DISTANCE(int distance);
unsigned char* TO_ASCII(int n,unsigned char* arr);