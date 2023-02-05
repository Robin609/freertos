#include "matrix_key.h"
#include <string.h>

#define delay_us Delay_us
#define delay_ms Delay_ms

/*ȫ�ֱ���*/
extern u8 i ;
extern u8 Passwd[] ;
extern u8 ascci_Passwd[] ;
extern u8 Input_pwd[] ;
extern u8 buf[] ;

/* ����һ */

/*******************************************************************************
*������ԭ�ͣ�void Matrix_ssKey_Pin_Init(void)
*�����Ĺ��ܣ�����������ų�ʼ������
*�����Ĳ�����None
*��������ֵ��None
*������˵���� ���ߣ�PB8-PB11 ���ߣ�PB12-PB15
*������д�ߣ���
*������д���ڣ�2022/3/19
*�����İ汾�ţ�V1.0
********************************************************************************/
void Matrix_ssKey_Pin_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);

	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_8 | GPIO_Pin_9 | GPIO_Pin_10 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU ;  
	GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_12 | GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
	GPIO_Init(GPIOC,&GPIO_InitStructure);
}	

/*******************************************************************************
*������ԭ�ͣ�int Matrix_Key_Scan(void)
*�����Ĺ��ܣ�����ɨ�躯��
*�����Ĳ�����None
*��������ֵ��
	�ɹ������ذ���ֵ��ʧ�ܣ�-1
*������˵���� ���ߣ�PB8-PB11 ���ߣ�PB12-PB15
*������д�ߣ���
*������д���ڣ�2022/3/19
*�����İ汾�ţ�V1.0
********************************************************************************/
int Matrix_Key_Scan(void)           //11.16�Ѿ��Ĺ�ԭ��λ��1325769.11.10.13.15.14��3*4
{
	u8 temp = 0;
	int key_val = -1;
	
	GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);			//��������
	delay_us(10);

	temp=(GPIO_ReadInputData(GPIOC) >> 8)&0xff;	
	
	//û�а�������ʱɨ��
	if (temp == 0xf0) 
	{
			delay_ms(10); 
			GPIO_ResetBits(GPIOC,GPIO_Pin_8|GPIO_Pin_9|GPIO_Pin_10|GPIO_Pin_11);			//��������
			delay_us(1);
			temp=(GPIO_ReadInputData(GPIOC) >> 8)&0xff;	
		
			if (temp != 0xf0) //��������ʱ���Լ�ֵ���и�ֵ
			{
				//��һ��
				GPIO_Write(GPIOC,0);
				delay_ms(1);
				GPIO_Write(GPIOC,(uint16_t)(0xFE << 8)); 
				
				if(((GPIO_ReadInputData(GPIOC) >> 8) & 0XF0) != 0XF0)
				{
						delay_ms(20);//����

						if(((GPIO_ReadInputData(GPIOC) >> 8) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOC) >> 8) & 0XFE);		//���н���ɨ��
								switch(temp)
								{
										case 0xEE:  key_val = 1;   break;

										case 0xDE:  key_val = 2;   break;

										case 0xBE:  key_val = 3;   break;

										//case 0x7E:  key_val = 4;   break;

										default:    key_val = -1;   break;
								}
						}
				}
				
				//�ڶ���
				GPIO_Write(GPIOC,0);
				delay_ms(5);
				GPIO_Write(GPIOC,(uint16_t)(0xFD << 8));
				
				if(((GPIO_ReadInputData(GPIOC) >> 8) & 0XF0)!= 0XF0)
				{
						delay_ms(20);

						if(((GPIO_ReadInputData(GPIOC) >> 8) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOC) >> 8) & 0XFD);
								switch(temp)
								{
										case 0xED:  key_val = 4;   break;

										case 0xDD:  key_val = 5;   break;

										case 0xBD:  key_val = 6;   break;

										//case 0x7D:  key_val = 8;   break;

										default:    key_val = -1;   break;
								}
						}
				}
				
				//������
				GPIO_Write(GPIOC,0);
				delay_ms(5);
				GPIO_Write(GPIOC,(uint16_t)(0xFB << 8));
				
				if(((GPIO_ReadInputData(GPIOC) >> 8) & 0XF0) != 0XF0)
				{
						delay_ms(20);

						if(((GPIO_ReadInputData(GPIOC) >> 8) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOC) >> 8) & 0XFB);
								switch(temp)
								{
										case 0xEB:  key_val = 7;   break;

										case 0xDB:  key_val = 8;   break;

										case 0xBB:  key_val = 9;   break;

									//	case 0x7B:  key_val = 12;   break;

										default:    key_val = -1;   break;
								}
						}
				}
				
				//������
				GPIO_Write(GPIOC,0);
				delay_ms(5);
				GPIO_Write(GPIOC,(uint16_t)(0xF7 << 8));
				
				if(((GPIO_ReadInputData(GPIOC) >> 8) & 0XF0) !=0XF0)
				{
						delay_ms(20);

						if(((GPIO_ReadInputData(GPIOC) >> 8) & 0XF0) != 0XF0)
						{
								temp=((GPIO_ReadInputData(GPIOC) >> 8) & 0XF7);
								switch(temp)
								{
										case 0xE7:  key_val = 10;   break;

										case 0xD7:  key_val = 11;   break;

										case 0xB7:  key_val = 12;   break;

								//		case 0x77:  key_val = 16;   break;

										default:    key_val = -1;   break;
								}
						}
					}
				}
	}
	if(key_val>0&&key_val<13)
	printf("�������%d\n",key_val);
	return key_val;

}



/* ������ */

/*******************************************************************************
*������ԭ�ͣ�void KEY_Init(void)
*�����Ĺ��ܣ��������ų�ʼ������
*�����Ĳ�����None
*��������ֵ��None
*������˵���� ���ߣ�PA0-PB3 ���ߣ�PA4-PA5
*������д�ߣ���
*������д���ڣ�2021/3/2
*�����İ汾�ţ�V1.0
********************************************************************************/
void KEY_Init(void)
{
	
	GPIO_InitTypeDef GPIO_InitStructure;

 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA,ENABLE);//ʹ��PORTAʱ��
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3;//PA 0 1 2 3
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP; //���ó��������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
	
	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_4|GPIO_Pin_5|GPIO_Pin_6|GPIO_Pin_7;//PA 4 5 6 7
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOA, &GPIO_InitStructure);//��ʼ��GPIOA
	
} 

/*******************************************************************************
*������ԭ�ͣ�int key_scan(void)
*�����Ĺ��ܣ�����ɨ�躯��
*�����Ĳ�����None
*��������ֵ��
	�ɹ������ذ���ֵ��ʧ�ܣ�0
*������˵���� ���ߣ�PA0-PB3 ���ߣ�PA4-PA5
*������д�ߣ���
*������д���ڣ�2021/3/2
*�����İ汾�ţ�V1.0
********************************************************************************/
int key_scan(void)
{
	static u8 key_up=1;
	int key_num=0;
	u8 temp=0;								//�ж��е����ݻ�����
	u8 temp1=0;								//�ж��е����ݻ�����
	GPIO_ResetBits(GPIOA,GPIO_Pin_0|GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3);			//��������
	delay_us(10);
	temp=GPIO_ReadInputData(GPIOA)&0xff;				//����A�����ݼĴ�����ֵ
	delay_us(1);
	if (key_up&&(temp != 0xf0))							//�жϼ��Ƿ������Ƿ�����
	{
		delay_ms(10);						//����
		key_up=0;							//���м�����ʱ����������־λ����
		temp = GPIO_ReadInputData(GPIOA)&0xff;			//���������µõ�A��ֵ
		if(temp != 0xf0)
		{			
			switch(temp)								//������̰��µ���Ӧ����
			{
				case 0xe0:
				{
					
					GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);		//���δӵ�һ�����͵������У�����A�����ݣ��Ӷ��ж�
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);										//����һ�еļ��̰��£��Ӷ�������Ӧ�ļ�ֵ
					delay_us(10);
					temp1=GPIO_ReadInputData(GPIOA)&0xff;
					if (temp1==0xee)
						key_num=1;
					else
					{
						GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
						GPIO_ResetBits(GPIOA,GPIO_Pin_1);
						delay_us(1);
						temp1=GPIO_ReadInputData(GPIOA)&0xff;
					
						if (temp1==0xed)
							key_num=5;
						else
						{
							GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
							GPIO_ResetBits(GPIOA,GPIO_Pin_2);
							delay_us(1);
							temp1=GPIO_ReadInputData(GPIOA)&0xff;
						
							if (temp1==0xeb)
								key_num=9;
							else
								key_num=13;
						}
					}
				}
				break;
				case 0xd0:
				{
					GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);
					delay_us(1);
					temp1=GPIO_ReadInputData(GPIOA)&0xff;
					if (temp1==0xde)
						key_num=2;
					else
					{
							GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
							GPIO_ResetBits(GPIOA,GPIO_Pin_1);
							delay_us(1);
							temp1=GPIO_ReadInputData(GPIOA)&0xff;
						
							if (temp1==0xdd)
								key_num=6;
							else
							{
								GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
								GPIO_ResetBits(GPIOA,GPIO_Pin_2);
								delay_us(1);
								temp1=GPIO_ReadInputData(GPIOA)&0xff;
							
								if (temp1==0xdb)
									key_num=10;
								else
									key_num=14;
							}
					}
				}
				break;
				case 0xb0:
				{
					GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);
					delay_us(1);
					temp1=GPIO_ReadInputData(GPIOA)&0xff;
					if (temp1==0xbe)
						key_num=3;
					else
					{
							GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
							GPIO_ResetBits(GPIOA,GPIO_Pin_1);
							delay_us(1);
							temp1=GPIO_ReadInputData(GPIOA)&0xff;
						
							if (temp1==0xbd)
								key_num=7;
							else
							{
								GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
								GPIO_ResetBits(GPIOA,GPIO_Pin_2);
								delay_us(1);
								temp1=GPIO_ReadInputData(GPIOA)&0xff;
							
								if (temp1==0xbb)
									key_num=11;
								else
									key_num=15;
							}
					}
				}
				break;
				case 0x70:
				{
					GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
					GPIO_ResetBits(GPIOA,GPIO_Pin_0);
					delay_us(1);
					temp1=(GPIO_ReadInputData(GPIOA)&0xff);
					if (temp1==0x7e)
						key_num=4;
					else
					{
						GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
						GPIO_ResetBits(GPIOA,GPIO_Pin_1);
						delay_us(1);
						temp1=GPIO_ReadInputData(GPIOA)&0xff;
					
						if (temp1==0x7d)
							key_num=8;
						else
						{
							GPIO_SetBits(GPIOA,GPIO_Pin_1|GPIO_Pin_2|GPIO_Pin_3|GPIO_Pin_0);
							GPIO_ResetBits(GPIOA,GPIO_Pin_2);
							delay_us(1);
							temp1=GPIO_ReadInputData(GPIOA)&0xff;
						
							if (temp1==0x7b)
								key_num=12;
							else
								key_num=16;
						}
					}
				}
				break;
				default :return 0;
			}		
		}
	}
	else if(temp == 0xf0)
	{												//��û�м����º󣬽�������־��λ
		key_up=1;
	}
	return key_num;	
}

/*�õ����������*/
void get_pwd(u16 pwd)
{
    Input_pwd[i] = pwd;
    i++;
}

/*�Ա�����*/
void verify (void)  
{
	uint16_t key_val=0;
	printf("��ʼ��������\r\n\n");
	while (1)
	{
		key_val = Matrix_Key_Scan();
			if (key_val > 0 && key_val < 10)
			{
				printf("This is S%d key\r\n",key_val);
				get_pwd(key_val);
				if(i>=6)
				{
					printf ("�������������");
					for(i=0;i<6;i++)
					{
						printf (" %d",Input_pwd[i]);
					}
					printf ("\r\n");
					i=0;
					if(!memcmp(Input_pwd,Passwd,6))
					{
						printf("�����ɹ������\r\n");
						break;
					}
					else
					{
						printf("gun\r\n");
						break;
					}
				}
			}
	}

}

/*�޸�����*/

void change_pwd (void)
{
	printf("��ʼ�޸�������\r\n\n");
	u16 c = -1, t =0;
//  u16	key_val=0 ;

	if(t==0)
	printf ("�����������\r\n");
	while(1)
	{
	c = Matrix_Key_Scan();
			if (c > 0 && c < 10)
			{
				printf("�������%d key\r\n",c);
				get_pwd(c);
				if(i>5)
				{
					printf ("�������������");
					for(i=0;i<6;i++)
					{
						printf (" %d",Input_pwd[i]);
					}
					printf ("\r\n");
					//i=0;
					if(!memcmp(Input_pwd,Passwd,6))
					{
						printf ("��������ȷ\r\n");
						printf ("����������\r\n");
						while(1)
						{
							c = Matrix_Key_Scan();
							if (c > 0 && c < 10)
							{
								printf("�������%d \r\n",c);
				        get_pwd(c);
							}
							if(i>5)
							{
								for(u16 q=0; q<6;q++)
								{
								  Passwd[q]=Input_pwd[q];
									printf("%d",Passwd[q]);
								}
								printf("\n�޸ĳɹ�\r\n");
								break;
							}
							//Delay_ms(50);
						}
					}
					else
					{
						printf("gun\r\n");
						break;
					}
					i=0;
				}
			}
		}
}

