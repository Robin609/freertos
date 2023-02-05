#ifndef __TIMER_H
#define __TIMER_H
#include "sys.h"

void TIM3_Int_Init(u16 arr,u16 psc);

typedef struct 
{
	unsigned char	Flag10ms;		//10ms±êÖ¾Î»
	unsigned char	Flag20ms;		//20ms
	unsigned char	Flag500ms;	//500ms
	unsigned char	Flag1s;	  	//1s
}Time;

extern Time SystemTime;
 
#endif
