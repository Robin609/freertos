#include "RT_delay.h"

//��ʱnus
//nusΪҪ��ʱ��us��.		    								   
void Delay_us(u32 nus)
{		
	vTaskDelay(nus*5);   /* ��ʱ500��tick */	 
}
//��ʱnms
//ע��nms�ķ�Χ
//SysTick->LOADΪ24λ�Ĵ���,����,�����ʱΪ:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK��λΪHz,nms��λΪms
//��72M������,nms<=1864 
void Delay_ms(u16 nms)
{	 		  	  
	 vTaskDelay(nms);   /* ��ʱ500��tick */	 	    
} 

