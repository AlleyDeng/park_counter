#ifndef UART_H
#define UART_H

#include "lpcreg.h"

extern unsigned int capacity;
extern unsigned int isAllow;
extern char dis_data[17];

void UsartConfiguration();
void sendString(unsigned char *str);
void runComm(char *recvBuf);
void setSpaceComm(char *num);
void setAllow(char *comm);
void barUpComm(char *comm);
void barDownComm(char *comm);
void stringUper(char *str);
void sendMessage(char *comm);

#endif