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
	
	ultrasonic_Init();         //��������عܽų�ʼ����PB8=trig;PB9=echo
}

void System_Init(void)
{
	//uart_init(9600);    //��ʼ������USART1�����������õľ���
	//printf("\r\nHC-SR04��ʼ����ʼ\r\n");
//	delay_init();
	printf("���ڳ�ʼ��������ģ��\r\n");
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	TIM3_Int_Init(9,7199);												  //1000hz�ļ���Ƶ�ʣ�������10Ϊ1ms��1ms�ж�һ�� 
	TIME4_Init(99,17);              //��ʼ����ʱ��TIM3�����ڲɼ���������������ʱ��
	TIME2_Init(39,17);              //��ʼ����ʱ��TIM2�����ھ�׼��ʱ	          
	printf("\r\nHC-SR04��ʼ�����\r\n");
	//SysTick_Init();                 //ϵͳʱ�����ڶ�ʱ���ͳ�����
	//IWDG_Init(4,625);   													  //���Ƶ��Ϊ64,����ֵΪ625,���ʱ��Ϊ1s	
	
}
