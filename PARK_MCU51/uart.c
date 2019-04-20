#include "uart.h"
#include "lcd.h"
#include "park.h"
#include <string.h>
#include <stdlib.h>

void sendMessage(char *comm)
{
	EA = 0;
	if (strncmp(comm, "GETMSG", 6) == 0)
	{
		// do it
		char temp[20];
		sendString("OPEN\n");
		sprintf(temp, "M%.1d%03d\n", isAllow, capacity);
		sendString(temp);
		
		delay_ms(10);
		sendString("OK\n");
	}
	EA = 1;
}

// 将字符串中小写字母转换为大写字母
void stringUper(char *str)
{
	EA = 0;
	if (str == NULL)
	{
		return;
	}
	while (*str != 0)
	{
		if (*str >= 'a' && *str <= 'z')
		{
			*str -= 32;
		}
		str++;
	}
	EA = 1;
}

// 设置剩余车位数量
void setSpaceComm(char *numStr)
{
	int num = 0;
	EA = 0;
	sendString("SETSPACE\n");
	while (*numStr != 0)
	{
		num = num * 10 + *numStr - '0';
		numStr++;
	}
	if (num <= MAXSIZE)
	{
		char temp[20];
		
		capacity = num;
		sprintf(dis_data, "   SPACE: %03d   ", capacity);
		LcdWriteCom(0x80);
		LcdWriteStr(dis_data);	
		LcdWriteCom(0x80+0x40);
		LcdWriteStr("                ");
		
		delay_ms(10);
		sendString("OK\n");
		delay_ms(10);
		sprintf(temp, "M%.1d%03d\n", isAllow, capacity);
		sendString(temp);
	}
	EA = 1;
}

// 设置使用状态
void setAllow(char *comm)
{
	char temp[20];
	EA = 0;
	sendString("SETALLOWANCE\n");	
	sendString(comm);
	sendString("\n");
	if (strncmp(comm, "ALLOW", 5) == 0)
	{
		isAllow = 1;
		delay_ms(10);
		sendString("OK\n");
	}
	if (strncmp(comm, "DENNY", 5) == 0)
	{
		isAllow = 0;
		delay_ms(10);
		sendString("OK\n");
	}
	if (strncmp(comm, "SET", 3) == 0)
	{
		if (isAllow == 0)
		{
			isAllow = 1;
		}
		else 
		{
			isAllow = 0;
		}
		delay_ms(10);
		sendString("OK\n");
	}
	
	if (isAllow == 0)
	{
		LcdWriteCom(0x80);
		LcdWriteStr("    NO ENTRY!   ");
	}
	else
	{
		LcdWriteCom(0x80);
		LcdWriteStr(dis_data);
	}
		
	sprintf(temp, "M%.1d%03d\n", isAllow, capacity);
	sendString(temp);
	delay_ms(10);
	
	EA = 1;
}

// 运行命令
void runComm(char *recvBuf)
{	// 此处默认接收字符串第一个字符为数字，选择功能
	unsigned char dataHead = *recvBuf - '0';
	EA = 0;
	stringUper(recvBuf);
	switch(dataHead)
	{
		case 0:
			sendMessage(recvBuf+1);
			break;
		case 1:		// 设置剩余车位数量
			setSpaceComm(recvBuf+1);		
			break;
		case 2:		// 设置通行状态
			setAllow(recvBuf+1);	
			break;
		case 3:		// 抬杆
			bar_up(3000);
			break;
		case 4:		// 降杆
			bar_down(3000);
			break;
		case 5:		// 进入
			enter_into();
			break;
		case 6:	    //
			leave_out(recvBuf+1);
			break;
		default:
			break;	
	}
	EA = 1;
}

// 发送字符串
void sendString(unsigned char *str)  
{                
    EA = 0;     
    while(*str != 0) 
    {                      
        SBUF = *str;
        while(TI == 0);
        TI=0;    
        str++;
    }
	SBUF = *str;
    while(TI == 0);
    TI=0; 
    EA = 1; 
}  

void UsartConfiguration()
{
	SCON = 0x50;  
	TMOD = 0x20;  
	/* // 12M 4800bps
	PCON = 0x80; 
	TH1 = 0xF3;
	TL1 = 0xF3;
	*/
	// 11.0592M 4800bps
	TH1 = 0xFA;	 
	TL1 = 0xF4;
	
	ES = 1;
	EA = 1;
	TR1 = 1;
	TI = 1;
	RI = 1;
}

void Usart() interrupt 4
{
	unsigned char recvData;
	static unsigned char i = 0;
	unsigned char recvFlag = 0;
	unsigned char recvLen = 0;
	char recvBuf[17];
	
	if (RI == 1)
	{
		RI = 0;
		recvData = SBUF;
		if (recvData != '\n' && recvData != '\r' && i < 16)
		{
			recvBuf[i++] = recvData;
		}
		else 
		{
			recvBuf[i] = '\0';
			recvLen = i;
			i = 0;
			recvFlag = 1;
		}
	}
	if (recvFlag == 1)
	{
		runComm(recvBuf);
		recvFlag = 0;
	}
}