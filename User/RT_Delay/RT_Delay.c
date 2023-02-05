#include "RT_delay.h"

//延时nus
//nus为要延时的us数.		    								   
void Delay_us(u32 nus)
{		
	vTaskDelay(nus*5);   /* 延时500个tick */	 
}
//延时nms
//注意nms的范围
//SysTick->LOAD为24位寄存器,所以,最大延时为:
//nms<=0xffffff*8*1000/SYSCLK
//SYSCLK单位为Hz,nms单位为ms
//对72M条件下,nms<=1864 
void Delay_ms(u16 nms)
{	 		  	  
	 vTaskDelay(nms);   /* 延时500个tick */	 	    
} 

