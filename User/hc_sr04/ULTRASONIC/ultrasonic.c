#include "ultrasonic.h"

u32 time;         //���ڲɼ�����������ʱ��ļ���
u32 delay_u;
	
//////////////////////////////////////////////////
//
//     ������   ultrasonic_Init()
//     ���룺   ��
//     �����   ��
//     ���ܣ�   �������ܽų�ʼ��
//     ˵����   ��
//
////////////////////////////////////////////////
void ultrasonic_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB,ENABLE);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_0; //trig
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed=GPIO_Speed_50MHz;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_InitStructure.GPIO_Pin=GPIO_Pin_1;  //echo
	GPIO_InitStructure.GPIO_Mode=GPIO_Mode_IN_FLOATING;
	GPIO_Init(GPIOB,&GPIO_InitStructure);
	
	GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	
}

///////////////////////////////////////////////////
//
//     ������   TIME4_Init()
//     ���룺   ��
//     �����   ��
//     ���ܣ�   ��ʼ����ʱ��3���ö�ʱ�����ڲɼ�������
//              ���ص�ʱ��
//     ˵����   �ú��������˶�ʱ��TIM3�ĸ����жϣ�
//              ÿ100us�ж�һ�Σ������ԣ��ü��ʱ��
//              �²�õľ����׼ȷ
//
////////////////////////////////////////////////////
void TIME4_Init(u16 arr,u16 psc)
{
	NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM4_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_Period=arr;                       //��������
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                    //Ԥ��Ƶ��
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM4,&TIM_TimeBaseInitStructure);
	
  TIM_ITConfig(TIM4,TIM_IT_Update,ENABLE);                       //�򿪶�ʱ��3�ĸ����ж�
}

//////////////////////////////////////////////////
//
//     ������   TIME2_Init()
//     ���룺   ��
//     �����   ��
//     ���ܣ�   ��ʼ����ʱ��2�����ھ�׼��ʱ
//     ˵����   �ú��������˶�ʱ���ĸ����жϣ�
//              ÿ10us�ж�һ��
//
/////////////////////////////////////////////////
void TIME2_Init(u16 arr,u16 psc)
{
  NVIC_InitTypeDef NVIC_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;

  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);
	
	NVIC_InitStructure.NVIC_IRQChannel=TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd=ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority=0;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority=0;
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision=0;
	TIM_TimeBaseInitStructure.TIM_CounterMode=TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period=arr;                        //��������Ϊ40
	TIM_TimeBaseInitStructure.TIM_Prescaler=psc;                     //Ԥ��Ƶϵ��Ϊ18
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter=0;
	TIM_TimeBaseInit(TIM2,&TIM_TimeBaseInitStructure);
	
	TIM_ClearFlag(TIM2,TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2,TIM_IT_Update,ENABLE);                        //�򿪶�ʱ�������ж�
}

//////////////////////////////////////////////////
//
//     ������   delay_usx()
//     ���룺   time1����ʱ���ٸ�10us��
//     �����   ��
//     ���ܣ�   ������ʱ X*10 ΢��
//     ˵����   ��TIM2��TIM2ÿ10us�ж�һ�Σ�ÿ��
//              �жϣ�delay_u����һ���ﵽ��ʱĿ��
//
////////////////////////////////////////////////////
void delay_usx(u32 time1)
{
	delay_u=time1;
	TIM_Cmd(TIM2,ENABLE);
	while(delay_u>0);
	TIM_Cmd(TIM2,DISABLE);
}

///////////////////////////////////////////////////
//
//     ������   GetDistance()
//     ���룺   ��
//     �����   ��
//     ���ܣ�   ����ɼ����ľ��벢������ʾ
//     ˵����   1������ԭ��trig����10us�󣬴�TIM3
//                 ���ɼ����������ص�ʱ�䣬Ȼ��ͨ��
//                 ���㹫ʽ��þ���
//              2����õľ��������ξ����ƽ��ֵ
//
/////////////////////////////////////////////////////
void GetDistance(void)
{
	int i;
//	printf("shengbo ing\n");
  for(i=5;i>0;i--)                                    //ѭ�����βɼ�����
	{
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
		GPIO_SetBits(GPIOB,GPIO_Pin_0);                     //����trig
		Delay_ms(10);                                       //����10us
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);                   //����trig
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==0);  //�ȴ�echo�ܽ�����ߵ�ƽ���TIM3��ʱ
		TIM_Cmd(TIM4,ENABLE);                               //��TIM3����ʼ��ȡ���������ص�ʱ��
		while(GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_1)==1);  //�ȴ�����ʱ��Ľ���
		TIM_Cmd(TIM4,DISABLE);                              //�ر�TIM3������ʱ��Ĳɼ�
		count=TIM_GetCounter(TIM3);                         //��ȡTIM3�ĸôμ���ֵ������ȡTIM3�Ĵ���CNT
		Distance0=calculer(count);	                        //���ú����������
		Distance1=Distance1+Distance0;                      //���������ۼӣ����ں����������ƽ��ֵ
		Delay_ms(100);                                    
	}	
	//printf("shengbo ed\n");		
	Distance1=Distance1/6;                                //����ƽ��ֵ
	Display(Distance1);                                   //���ú�����ͨ��ָʾ�Ʒ������뷶Χ
	printf("\r\nDistance:%d.",Distance1/100);                 //�������
	printf("%dcm\n\r\n",Distance1%100);
	Delay_ms(100);
}

///////////////////////////////////////////////////////
//
//     ������   calculer()
//     ���룺   count��TIM3�ôεļ���ֵ��
//     �����   Distance������õ��ľ��룬��
//     ���ܣ�   �������ĺ��Ĺ�ʽ
//     ˵����   1������õ��ľ��뵥λ��cm�����ҷŴ���100��
//              2�����㹫ʽ�ǣ�TIM3�ļ�������*18/72000000*340/2*100*100
//                 ����18ΪTIM3��Ԥ��Ƶϵ����72000000Ϊϵͳ����Ƶ��
//                 340Ϊ���٣�����2����Ϊ���أ�100�����ٵ��׻��������
//                 ��һ��100�ǷŴ�100��
//
////////////////////////////////////////////////////////
u32 calculer(u32 count)
{
  u32 Distance;
	Distance=((float)(count+100*time)*0.425);   //������㹫ʽ��������ʽ
	time=0;                                     //���TIM3���жϴ���
	
	return Distance;
	
}

//////////////////////////////////////////////////////
//
//     ������   Display()
//     ���룺   Distance���������ɼ����ľ����С��
//     �����   ��
//     ���ܣ�   �����뷶Χ��ָʾ����ʾ
//     ˵����   �ú���ͨ��ָʾ�ƽ��������ɼ����ľ���
//              ��Χ��������
//
//////////////////////////////////////////////////////
void Display(u32 Distance)
{
  if(Distance>20000)                               //����������2m����LED1��
	{
	  GPIO_SetBits(GPIOB,GPIO_Pin_1|GPIO_Pin_5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_0);
	}
	else if((Distance>10000)&&(Distance<=20000))     //����������1m��С��2m����LED2��
	{
	  GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_5);
		GPIO_ResetBits(GPIOB,GPIO_Pin_1);
	}
	else                                             //�������С��1m����LED3��
	{
	  GPIO_SetBits(GPIOB,GPIO_Pin_0|GPIO_Pin_1);
		GPIO_ResetBits(GPIOB,GPIO_Pin_5);
	}
}

/////////////////////////////////////////////////////
//
//     ������   TIM3_IRQHandler()
//     ���룺   ��
//     �����   ��
//     ���ܣ�   TIM3�жϺ���
//     ˵����   ÿ100us����ú�������ȡ����������ʱ������
//              TIM3�жϵĴ��������ڼ���ʱ������ľ���
//
/////////////////////////////////////////////////////
void TIM4_IRQHandler(void)
{
  if(TIM_GetFlagStatus(TIM4,TIM_FLAG_Update)!=RESET)
	{
	  time++;                                    //�жϱ�־�Լ�1
		TIM_ClearFlag(TIM4,TIM_FLAG_Update);	     //����жϱ�־
	}
}

///////////////////////////////////////////////////
//
//     ������   TIM2_IRQHandler()
//     ���룺   ��
//     �����   ��
//     ���ܣ�   TIM2�жϺ���
//     ˵����   ÿ10us����ú�����ÿ����ú�����
//              delay_u�Լ�1��������0ʱ����ʱ����
//              
/////////////////////////////////////////////////////
void TIM2_IRQHandler(void)
{
  if(TIM_GetFlagStatus(TIM2,TIM_FLAG_Update)!=RESET)
	{
		delay_u--;                               //��ʱ��־�Լ�1
		TIM_ClearFlag(TIM2,TIM_FLAG_Update);     //����жϱ�־
	}
}

