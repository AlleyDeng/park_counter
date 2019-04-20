#include "park.h"
#include "key.h"
#include "main.h"
#include "lpcreg.h"
#include "mfrc522.h"
#include <stdio.h>
#include <string.h>

unsigned int capacity = MAXSIZE;
unsigned int isAllow = 1;
char dis_data[17] = "   SPACE: 100   ";

unsigned char code data1[16] = {0xFF,0xFF,0xFF,0xFF,0x00,0x00,0x00,0x00,0xFF,0xFF,0xFF,0xFF,0x01,0xFE,0x01,0xFE};
//M1卡的某一块写为如下格式，则该块为钱包，可接收扣款和充值命令
//4字节金额（低字节在前）＋4字节金额取反＋4字节金额＋1字节块地址＋1字节块地址取反＋1字节块地址＋1字节块地址取反 
unsigned char code data2[4]  = {0,0,0,0x01};
unsigned char code DefaultKey[6] = {0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF}; 


unsigned char g_ucTempbuf[20];                         

void InitializeSystem()
{
    P0M1 = 0x0; P0M2 = 0x0;
    P1M1 = 0x0;	P1M2 = 0x0;                      
    P3M1 = 0x0; P3M2 = 0xFF; 
    P0 = 0xFF; P1 = 0xFF; P3 = 0xFF;P2 = 0xFF;
}

void main (void)
{
	unsigned char status, i;
	unsigned int temp;

	UsartConfiguration();
	LcdInit();
	IntConfiguration();
	
	InitializeSystem();		
    PcdReset();
    PcdAntennaOff(); 
    PcdAntennaOn();
	
	sprintf(dis_data, "   SPACE: %03d   ", capacity);

	P10 = 0; 
	P11 = 0;
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
	
	while ( 1 )
    {   
		LED0 = 1;
        status = PcdRequest(PICC_REQALL, g_ucTempbuf);

        if (status != MI_OK)
        {  
			UsartConfiguration();
			PcdReset();
            PcdAntennaOff(); 
            PcdAntennaOn();
            continue;
        }
        status = PcdAnticoll(g_ucTempbuf); 
        if (status != MI_OK)
        {   
            continue;    
        }
        printf("UID:");
		for(i=0;i<4;i++)
        {
            temp=g_ucTempbuf[i];
            printf("%X",temp);

        }
		printf("\n");
		LED0 = 0;

        status = PcdSelect(g_ucTempbuf);
        if (status != MI_OK)
        {    
            continue;    
        }
        status = PcdAuthState(PICC_AUTHENT1A, 1, DefaultKey, g_ucTempbuf);
        if (status != MI_OK)
        {    
            continue;    
        }

        status = PcdWrite(1, data1);
        if (status != MI_OK)
        {    
            continue;    
        }

        status = PcdValue(PICC_DECREMENT,1,data2);
        if (status != MI_OK)
        {    
            continue;    
        }

        status = PcdBakValue(1, 2);
        if (status != MI_OK)
        {    
            continue;    
        }

        status = PcdRead(2, g_ucTempbuf);
        if (status != MI_OK)
        {   
            continue;			
        }
/*
		printf("Data: ");
		for(i=0;i<16;i++)
        {
			temp=g_ucTempbuf[i];
			printf("%X",temp);
		}	
		printf("\n");		
*/
		printf("LED\n");
		LED0 = 1;
		PcdHalt();
    }
}