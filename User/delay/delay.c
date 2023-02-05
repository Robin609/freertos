
/*-----------------------------------------------------*/
/*                                                     */
/*         		  	  延时函数                         */
/*                                                     */
/*-----------------------------------------------------*/


/*
 *	delay_us()是us级延时函数，delay_ms和delay_xms()都是ms级的延时函数,delay_us()和
 *	delay_xms()不会导致任务切换。delay_ms()其实就是对 FreeRTOS中的延时函数vTaskDelay()的
 *	简单封装，所以在使用delay_ms()的时候就会导致任务切换。 
 */

		  
#include "delay.h"

/*函数名 us延时（freertos vTaskDelay()封装）*/
void delay_us(u32 ums)
{	 		  	  
	
		
			vTaskDelay(ums*5);
				
	
	
}

/*-------------------------------------------------*/
/*函数名：ms延时 (FreeRTOS vTaskDelay()封装)        */
/*参  数  ms                                       */
/*返回值：无                                        */
/*-------------------------------------------------*/

void delay_ms(u16 nms)
{	 		  	 
	
	vTaskDelay(nms);		   
}

////延时nms,不会引起任务调度
////nms:要延时的ms数
//void delay_xms(u32 nms)
//{
//	u32 i;
//	for(i = 0; i < nms; i++) delay_xus(1000);
//}







































