#ifndef __ULTRASONIC_H
#define __ULTRASONIC_H


#include "stm32f10x.h"
#include "bsp_usart.h"
#include  "RT_Delay.h"



extern  volatile u32 count,Distance0,Distance1;

void ultrasonic_Init(void);
void TIME4_Init(u16 arr,u16 psc);
u32 calculer(u32 count);
void TIME2_Init(u16 arr,u16 psc);
void Display(u32 Distance);
void TIM2_IRQHandler(void);
void TIM4_IRQHandler(void);
void GetDistance(void);
void delay_usx(u32 time1);


#endif


