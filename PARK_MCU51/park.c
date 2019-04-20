#include "park.h"

void delay_ms(unsigned int ms)
{
	unsigned int i, j;
	
	for (i = ms; i > 0; i--)
	{
		for (j = 110; j > 0; j--);
	}
}

void enter_into(void)
{
	unsigned char i;
	char str[10];
	if (capacity > 0 && isAllow)
	{
		EA = 0;
		LcdWriteCom(0x80+0x40);
		LcdWriteStr("    Welcome!    ");
		bar_up(3000);
		delay_ms(8000);
		bar_down(3000);	
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
		
	
		EA = 1;
	}
	else if (capacity == 0)
	{
		LcdWriteCom(0x80+0x40);
		LcdWriteStr("THE PARK IS FULL");
	}

}

void leave_out(char *buf)
{
	unsigned char i;
	char str[10];
	if (capacity < MAXSIZE && isAllow)
	{
		EA = 0;
		LcdWriteCom(0x80+0x40);
		LcdWriteStr(buf);
		bar_up(3000);
		delay_ms(8000);
		bar_down(3000);
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
		
		EA = 1;
	}	
}

void bar_up(unsigned int nms)
{
	sendString("BARUP\n");
	EA = 0;
	P10 = 0;
	P11 = 1;
	delay_ms(nms);
	P11 = 0;
	sendString("OK\n");
	EA = 1;
}

void bar_down(unsigned int nms)
{
	sendString("BARDOWN\n");
	EA = 0;
	P10 = 1;
	P11 = 0;
	delay_ms(nms);
	P10 = 0;
	sendString("OK\n");
	EA = 1;
}
