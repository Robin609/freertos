//#include "led.h"
//#include "wdg.h"
#include "timer.h"
#include "delay.h"
#include "stm32f10x.h"
#include "System_Init.h"
#include "ultrasonic.h"
#include "SysTick.h"

volatile u32 count,Distance0,Distance1;

void System_Init_IO(void)
{
	//LED_Init();
	
	ultrasonic_Init();         //超声波相关管脚初始化：PB8=trig;PB9=echo
}

void System_Init(void)
{
	//uart_init(9600);    //初始化串口USART1，用于输出测得的距离
	//printf("\r\nHC-SR04初始化开始\r\n");
//	delay_init();
	printf("正在初始化超声波模块\r\n");
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	TIM3_Int_Init(9,7199);												  //1000hz的计数频率，计数到10为1ms，1ms中断一次 
	TIME4_Init(99,17);              //初始化定时器TIM3，用于采集超声波来回所用时间
	TIME2_Init(39,17);              //初始化定时器TIM2，用于精准延时	          
	printf("\r\nHC-SR04初始化完成\r\n");
	//SysTick_Init();                 //系统时钟用于定时发送超声波
	//IWDG_Init(4,625);   													  //与分频数为64,重载值为625,溢出时间为1s	
	
}
