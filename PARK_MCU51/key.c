#include "key.h"
#include "park.h"

//unsigned char KeyValue = 0;

/*
void KeyScan(void)
{
	unsigned char i;
	char str[10];
	if (KeyValue == 1 && capacity > 0 && isAllow)
	{
		EA = 0;
		LcdWriteCom(0x80+0x40);
		LcdWriteStr("    Welcome!    ");
		car_in_out();
		capacity--;
		sprintf(dis_data, "   SPACE: %03d   ", capacity);
		LcdInit();
		for(i=0;i<16 && dis_data[i] != 0;i++)
		{
			LcdWriteData(dis_data[i]);	
		}
		LcdWriteCom(0x80+0x40);
		LcdWriteStr("                ");
		
		sprintf(str, "M%.1d%03d\n", isAllow, capacity);
		sendString(str);
		
		KeyValue = 0;
		EA = 1;
	}
	else if (KeyValue == 1 && capacity == 0)
	{
		LcdWriteCom(0x80+0x40);
		LcdWriteStr("THE PARK IS FULL");
		KeyValue = 0;
	}
	if (KeyValue == 2 && capacity < MAXSIZE && isAllow)
	{
		EA = 0;
		LcdWriteCom(0x80+0x40);
		LcdWriteStr("   Bon voyage!  ");
		car_in_out();
		capacity++;
		sprintf(dis_data, "   SPACE: %03d   ", capacity);
		LcdInit();
		for(i=0;i<16 && dis_data[i] != 0;i++)
		{
			LcdWriteData(dis_data[i]);	
		}
		LcdWriteCom(0x80+0x40);
		LcdWriteStr("                ");
		
		sprintf(str, "M%.1d%03d\n", isAllow, capacity);
		sendString(str);
		
		KeyValue = 0;
		EA = 1;
	}	
}
*/
void IntConfiguration()
{
	IT0 = 1;
	EX0 = 1;
	
	IT1 = 1;
	EX1 = 1;
	
	EA = 1;	
}

void Int0()	interrupt 0		
{
	delay_ms(10);	 
	if(car_in == 0)
	{
		P10 = 0;
		P11 = 1;
		while (car_in == 0);
	}
	P11 = 0;
	//KeyScan();
}

void Int1() interrupt 2		
{
	delay_ms(10);		
	if(car_out == 0)
	{
		P10 = 1;
		P11 = 0;	
		while(car_out == 0);
	}
	P10 = 0;
	//KeyScan();
}