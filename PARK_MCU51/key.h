#ifndef KEY_H
#define KEY_H

#include "lpcreg.h"

sbit car_in = P3^2;
sbit car_out = P3^3;

extern unsigned int capacity;
extern unsigned int isAllow;
extern char dis_data[17];

void Delay(unsigned int n);
void IntConfiguration();
void KeyScan(void);

#endif