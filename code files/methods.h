#ifndef _METHODS
#define _METHODS
#include "stdint.h"
#include<stdio.h>
#include"tm4c123gh6pm.h"

void portF_init(void);
void portA_init(void);
char readChar(void);
void printChar(char c);
void printString(char* string);
void Timer3_init(int period);
void ADC_init(void);
void function(int in,int out);
#endif