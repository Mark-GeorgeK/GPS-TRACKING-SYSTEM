#include "Functions.h"

#define TURNING_INDIC 7

//Global Variables
int initialCounter = TURNING_INDIC;
float total_distance = 0;
float lon[TURNING_INDIC];
float lat[TURNING_INDIC];
float COG[TURNING_INDIC];
float startingPoint[3];
float currentCog = 0;
int main() {
	float latitude1, longitude1, latitude2, longitude2;

	//Initialization
	Init();
	UART_INIT();
	LCD_INIT();

	LCD_CMD(0X30);  //wake up
	msdelay(50);
	LCD_CMD(0X38);  //8-bit bus mode, 2 line display mode, 5x8 dots display mode
	msdelay(50);
	LCD_CMD(0X01);  //clear display
	msdelay(50);
	LCD_CMD(0X0F);  //turn on display
	msdelay(50);
	LCD_CMD(0X80);  //first row
	Greeting("Hello User..");
	msdelay(700);
	LCD_CMD(0X01);              //Youssef <- Delete this
	msdelay(250);

	// Gps call
    // GPSread();
    // while(!fix){}
	GPIO_PORTF_DATA_R |= 0x02; //light red when fix is set

	while (GPIO_PORTF_DATA_R & 0x01) {}   //To be checked by MARK whether it's negative logic or
	GPIO_PORTF_DATA_R &= ~(0x02); // red led off when turned on
	GPIO_PORTF_DATA_R |= 0x04; //blue led on
	Greeting("Distance=");
	msdelay(200);
	PRINT_DISTANCE((int)total_distance);

	do {
		GPSread();
		latitude1 = Latitude();
		longitude1 = Longitude();
	} while ((int)latitude1 != 30 || (int)longitude1 != 31);
	while (DestinationReached()) {
		msdelay(5000);
		GPSread();
		latitude2 = Latitude();
		longitude2 = Longitude();
		if ((int)latitude2 != 30 || (int)longitude2 != 31) { continue; }
		total_distance += fabs(DistanceBetween2Points(latitude1, longitude1, latitude2, longitude2));
		PRINT_DISTANCE((int)total_distance);
		latitude1 = latitude2;
		longitude1 = longitude2;
	}
	LED_ON();
	while (1) {}

	//	Init();
	//	UART_INIT();
	//	LCD_INIT();
	//
	//	//Print Hello
	//
	//
	//    LCD_CMD(0X30);  //wake up
	//	msdelay(50);
	//    LCD_CMD(0X38);  //8-bit bus mode, 2 line display mode, 5x8 dots display mode
	//    msdelay(50);
	//    LCD_CMD(0X01);  //clear display
	//    msdelay(50);
	//    LCD_CMD(0X0F);  //turn on display
	//    msdelay(50);
	//    LCD_CMD(0X80);  //first row
	//    LCD_display("Hello User..");
	//    msdelay(700);
	//    LCD_CMD(0X01);
	//    msdelay(250);
    //
	//	//Gps call
	//
	////	while(!GPSread()){}
	////	GPIO_PORTF_DATA_R |= 0x02;
	//
	//	while(GPIO_PORTF_DATA_R & 0x01){}	//To be checked by MARK whether it's negative logic or
	//	GPIO_PORTF_DATA_R &= ~(0x02);
	//	GPIO_PORTF_DATA_R |= 0x08;
	//
	//	//Loop
	//	//done Add First Distance Display
	//        LCD_display("Distance=");
	//
	//        msdelay(200);
	//		 PRINT_DISTANCE((int)total_distance);
	//
	//		while(initialCounter--){
	//			//scanf("%f", &COG[TURNING_INDIC-initialCounter-1]); //cin longs and lats
	//			GPSread();
	//
	//			lon[TURNING_INDIC-initialCounter-1] = Longitude();
	//			lat[TURNING_INDIC-initialCounter-1] = Latitude();
	//			COG[TURNING_INDIC-initialCounter-1] = CourseLand();
	//
	//			// cogRunningSum += COG[TURNING_INDIC-initialCounter-1]; //Remove
	//			// cogRunningNum++;
	//			if(initialCounter +1 ==TURNING_INDIC){
	//				startingPoint[0] = lat[0];
	//				startingPoint[1] = lon[0];
	//				startingPoint[2] = COG[0];
	//
	//			}
	// 	   }
	//		currentCog = Average(COG);
	//
	//		while(!DestinationReached()){
	//			GPSread();
	//			 if(Outlier(currentCog, CourseLand() ) )
	//            	continue;
	//
	//	        ShiftInsert(lon, Longitude() );
	//	        ShiftInsert(lat, Latitude() );
	//	        ShiftInsert(COG, CourseLand() );
	//
	//	        if(Turned(COG,currentCog)&&DegCheck(COG)){
	//				total_distance += DistanceBetween2Points(startingPoint[0], startingPoint[1],lat[0],lon[0]);	//Store point previous to it?
	//				startingPoint[0] = lat[0];
	//				startingPoint[1] = lon[0];
	//				startingPoint[2] = COG[0];
	//				//done LCD Displays "TURNED" for a brief delay
	//				//done Second Distance Display
	//				 PRINT_DISTANCE((int)total_distance);
	//                LCD_CMD(0XC0);  //force cursor to 2nd row
	//				msdelay(30);
	//				LCD_display("TURNED");
	//				msdelay(30);
	//				LCD_CMD(0XC0);
	//				msdelay(30);
	//				LCD_display("      ");  //unsure if this will clear second row as there is no direct function to do so except clear the whole display
	//
	//				msdelay(2000);
	//			}
	//		}
	//		total_distance += DistanceBetween2Points(startingPoint[0], startingPoint[1],lat[TURNING_INDIC-1],lon[TURNING_INDIC-1]);
	//		//done Third Distance Display
	//		PRINT_DISTANCE((int)total_distance);
	//
	//
	//	//Test case #1: Testing 7 segments functions and LED_ON functions
	//	//  while(!DestinationReached(distance)){
	//	//  	distance++;
	//	//  	SegmentsDisplay(distance);
	//	//    LED_ON(distance);
	//	//  	delay_1sec();
	//	//  }
	//
	//	//Test case #2: Testing 7 segments functions and Distance functions
	//	//distance=DistanceBetween2Points(30.064601,31.277308,30.064693,31.277582);
	//	//SegmentsDisplay(distance);
	//	while(1){}
}
