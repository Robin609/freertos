#ifndef __RT_DELAY_H
#define __RT_DELAY_H
#include "sys.h"
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
//////////////////////////////////////////////////////////////////////////////////	 

//RT延迟函数
//库函数版本例程
/********** mcudev.taobao.com 出品  ********/


//使用SysTick的普通计数模式对延迟进行管理
//包括delay_us,delay_ms

////////////////////////////////////////////////////////////////////////////////// 	 
void Delay_ms(u16 nms);
void Delay_us(u32 nus);
void Delay_M(u32 nus);
#endif





























