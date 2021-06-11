#include "Functions.h"

//#define TURNING_INDIC 7  //To be changed by Mario after practical trial

//Global Variables
float distance =0;


int main() {
	float latitude1, longitude1, latitude2, longitude2;
	
	//Initialization
	Init();			//TO BE MODIFIED BY MARK
	UART_INIT();
	LCD_INIT();

	//Print Hello

    
    LCD_CMD(0X30);  //wake up
    msdelay(50);
    LCD_CMD(0X38);  //8-bit bus mode, 2 line display mode, 5x8 dots display mode
    msdelay(50);
    LCD_CMD(0X01);  //clear display
    msdelay(50);
    LCD_CMD(0X0F);  //turn on display
    msdelay(50);
    LCD_CMD(0X80);  //first row
    LCD_display("Hello User..");
    msdelay(700);
    LCD_CMD(0X01);				//Youssef <- Delete this
    msdelay(250);
    



	//Gps call
	GPSread();
	while(!fix){}
	GPIO_PORTF_DATA_R |= 0x02; //light red when fix is set

	while(GPIO_PORTF_DATA_R & 0x01){}	//To be checked by MARK whether it's negative logic or
	GPIO_PORTF_DATA_R &= ~(0x02); // red led off when turned on
	GPIO_PORTF_DATA_R |= 0x04; //blue led on 
	LCD_display("Distance=");
    msdelay(200);
	PRINT_DISTANCE((int)distance);

	GPSread();
	latitude1= Latitude();
	longitude1= Longitude();
	
	while(!DestinationReached){
		
		GPSread();
		latitude2 = Latitude();
		longitude2 = Longitude();

		distance+= DistanceBetween2Points(latitude1,longitude1,latitude2,longitude2);

		LCD_display("Distance=");
		msdelay(200);
		PRINT_DISTANCE((int)distance);
		latitude1= latitude2;
		longitude1= longitude2;
	}
	


	while(1){}
}
