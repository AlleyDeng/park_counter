#ifndef PARK_H
#define PARK_H

#include "lpcreg.h"
#include <stdio.h>
#include <stdlib.h>
#include "lcd.h"
#include "uart.h"

#define MAXSIZE 99


sbit P10 = P1^0;
sbit P11 = P1^1;

extern unsigned int capacity;
extern unsigned int isAllow;
extern char dis_data[17];

void delay_ms(unsigned int ms);
void enter_into(void);
void leave_out(char *buf);
void bar_up(unsigned int nms);
void bar_down(unsigned int nms);

#endif