#ifndef __RT_DELAY_H
#define __RT_DELAY_H
#include "sys.h"
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
//////////////////////////////////////////////////////////////////////////////////	 

//RT�ӳٺ���
//�⺯���汾����
/********** mcudev.taobao.com ��Ʒ  ********/


//ʹ��SysTick����ͨ����ģʽ���ӳٽ��й���
//����delay_us,delay_ms

////////////////////////////////////////////////////////////////////////////////// 	 
void Delay_ms(u16 nms);
void Delay_us(u32 nus);
void Delay_M(u32 nus);
#endif





























