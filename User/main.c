/*
*************************************************************************
*                             ������ͷ�ļ�
*************************************************************************
*/ 
/* FreeRTOSͷ�ļ� */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"//�¼���־��
#include "semphr.h "
#include "queue.h"
/* ������Ӳ��bspͷ�ļ� */
#include "bsp_led.h"
#include "bsp_usart.h"
#include "bsp_key.h"
#include "bsp_as608.h"
#include "as608_test.h"
#include "matrix_key.h"
#include "RT_Delay.h"
#include "System_Init.h"
#include "timer.h"
#include "SysTick.h"
#include "ultrasonic.h"
#include "lcd_st7735.h"
/**************************** ������ ********************************/
/* 
 * ��������һ��ָ�룬����ָ��һ�����񣬵����񴴽���֮�����;�����һ��������
 * �Ժ�����Ҫ��������������Ҫͨ�������������������������������Լ�����ô
 * ����������ΪNULL��
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* ���������� */
static TaskHandle_t LED_Task_Handle = NULL;/* LED������ */
static TaskHandle_t KEY_Task_Handle = NULL;/* KEY������ */
static TaskHandle_t AS608_Task_Handle = NULL;/* AS608������ */
static TaskHandle_t CHOOSE_Task_Handle = NULL;/* AS608������ */

/********************************** �ں˶����� *********************************/
/*
 * �ź�������Ϣ���У��¼���־�飬�����ʱ����Щ�������ں˵Ķ���Ҫ��ʹ����Щ�ں�
 * ���󣬱����ȴ����������ɹ�֮��᷵��һ����Ӧ�ľ����ʵ���Ͼ���һ��ָ�룬������
 * �ǾͿ���ͨ��������������Щ�ں˶���
 *
 * �ں˶���˵���˾���һ��ȫ�ֵ����ݽṹ��ͨ����Щ���ݽṹ���ǿ���ʵ��������ͨ�ţ�
 * �������¼�ͬ���ȸ��ֹ��ܡ�������Щ���ܵ�ʵ��������ͨ��������Щ�ں˶���ĺ���
 * ����ɵ�
 * 
 */
 
SemaphoreHandle_t keyboard_Handle;
QueueHandle_t as608_Queue =NULL;
QueueHandle_t key_Queue =NULL;



/******************************* ȫ�ֱ������� ************************************/
/*
 * ��������дӦ�ó����ʱ�򣬿�����Ҫ�õ�һЩȫ�ֱ�����
 */
u8 i = 0;
u8 Passwd[6] = {1,2,3,4,5,6};
u8 ascci_Passwd[6] = {49,50,51,52,53,54};
u8 Input_pwd[6] = {0};
u8 buf[4] = {0x00,0x01,0x0B,0x11};


/***********�궨��**********/
#define  QUEUE_LEN 4
#define  QUEUE_SIZE 4

/*
*************************************************************************
*                             ��������
*************************************************************************
*/                                                                                                                                                      
static void AppTaskCreate(void);/* ���ڴ������� */

static void LED_Task(void* pvParameters);/* LED_Task����ʵ�� */
static void KEY_Task(void* pvParameters);/* KEY_Task����ʵ�� */
static void AS608_Task(void* pvParameters);/* AS608_Task����ʵ�� */
static void CHOOSE_Task(void* pvParameters);/* AS608_Task����ʵ�� */


static void BSP_Init(void);/* ���ڳ�ʼ�����������Դ */

/*****************************************************************
  * @brief  ������
  * @param  ��
  * @retval ��
  * @note   ��һ����������Ӳ����ʼ�� 
            �ڶ���������APPӦ������
            ������������FreeRTOS����ʼ���������
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  /* ������Ӳ����ʼ�� */
  BSP_Init();

  printf("����һ��FreeRTOS�������ʵ�飡\n");
  
//		LCD_Fill(1,   1 ,  8,   8,   WHITE);           // ��Ļ��䣬
//	LCD_Fill(121, 1 ,  129, 8,   RED);             // ͨ���Ŵ󾵹۲����Ͻǡ����Ͻ�����ɫ�飬�����������ص�X���꣬��߱��ڵ��˵�1�У��ұ߶���1��129
//	LCD_Fill(1,   152, 8  , 160, GREEN);
//	LCD_Fill(121, 152, 129, 160, BLUE);            // ͨ���۲��ϡ���ɫ�飬Y�����������������
//	LCD_String(6, 10,"test",24, YELLOW , BLACK);	
   /* ����AppTaskCreate���� */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* ������ں��� */
                        (const char*    )"AppTaskCreate",/* �������� */
                        (uint16_t       )512,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )1, /* ��������ȼ� */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* ������ƿ�ָ�� */ 
  /* ����������� */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* �������񣬿������� */
    return -1;  
  
  while(1);   /* ��������ִ�е����� */    
}


/***********************************************************************
  * @ ������  �� AppTaskCreate
  * @ ����˵���� Ϊ�˷���������е����񴴽����������������������
  * @ ����    �� ��  
  * @ ����ֵ  �� ��
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* ����һ��������Ϣ����ֵ��Ĭ��ΪpdPASS */
  
  taskENTER_CRITICAL();           //�����ٽ���
  
		/* ���� as608_Queue */
	  as608_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* ��Ϣ���еĳ��� */
														(UBaseType_t ) QUEUE_SIZE);/* ��Ϣ�Ĵ�С?
,��*/
  if (NULL != as608_Queue)
		printf(" ���� as608_Queue ��Ϣ���гɹ�!\r\n");
	
		/* ���� key_Queue */
	  key_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* ��Ϣ���еĳ��� */
														(UBaseType_t ) QUEUE_SIZE);/* ��Ϣ�Ĵ�С?
,��*/
  if (NULL != key_Queue)
		printf(" ���� key_Queue ��Ϣ���гɹ�!\r\n");
	
  /* ����LED_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )LED_Task, /* ������ں��� */
                        (const char*    )"LED_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )3,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&LED_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����LED_Task����ɹ�!\r\n");
	
  /* ����KEY_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )KEY_Task,  /* ������ں��� */
                        (const char*    )"KEY_Task",/* �������� */
                        (uint16_t       )1024,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )3, /* ��������ȼ� */
                        (TaskHandle_t*  )&KEY_Task_Handle);/* ������ƿ�ָ�� */ 
  if(pdPASS == xReturn)
    printf("����KEY_Task����ɹ�!\r\n");
	
  /* ����AS608_Task���� */
  xReturn = xTaskCreate((TaskFunction_t )AS608_Task,  /* ������ں��� */
                        (const char*    )"AS608_Task",/* �������� */
                        (uint16_t       )1024,  /* ����ջ��С */
                        (void*          )NULL,/* ������ں������� */
                        (UBaseType_t    )4, /* ��������ȼ� */
                        (TaskHandle_t*  )&AS608_Task_Handle);/* ������ƿ�ָ�� */ 
  if(pdPASS == xReturn)
    printf("����AS608_Task����ɹ�!\r\n");  
	
	/*����CHOOSE_task����*/
	  xReturn = xTaskCreate((TaskFunction_t )CHOOSE_Task, /* ������ں��� */
                        (const char*    )"LED_Task",/* �������� */
                        (uint16_t       )512,   /* ����ջ��С */
                        (void*          )NULL,	/* ������ں������� */
                        (UBaseType_t    )2,	    /* ��������ȼ� */
                        (TaskHandle_t*  )&CHOOSE_Task_Handle);/* ������ƿ�ָ�� */
  if(pdPASS == xReturn)
    printf("����CHOOSE_Task����ɹ�!\r\n");
	
	
  vTaskDelete(AppTaskCreate_Handle); //ɾ��AppTaskCreate����
  
  taskEXIT_CRITICAL();            //�˳��ٽ���
}


/**********************************************************************
  * @ ������  �� CHOOSE_Task
  * @ ����˵���� CHOOSE_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void CHOOSE_Task(void* parameter)			
{	
	BaseType_t xReturn = pdTRUE;
	uint32_t a = 0;
	//a=Matrix_Key_Scan();
 u32 q =0;

//			/*����STM32��ָ��ģ���ͨ��*/
//  Connect_Test();

	
	//LCD_CHINSES_GetPassWord(20,30);
		while (1)
		{
//		LCD_String(55, 140,"by robin",16, YELLOW , BLACK);//����ǩ��
//	  LCD_CHINSES_BYSJ(6,6);
//		LCD_CHINSES_GetPassWord(20,30);
			if(q==0)
			printf("�뿪ʼ����\r\n");
			q++;
			
			a = Matrix_Key_Scan();
//		key_val = key_scan();
		if (a == 10)
			{
			printf("you choose is keyborad\r\n");
			xReturn = xQueueSend( key_Queue, //��Ϣ���еľ��
																	&a,     //���͵�����
																0);	
			//break;
			}
		if (a == 12)
			{
			printf("you choose is fingerprint\r\n");
			xReturn = xQueueSend( as608_Queue, //��Ϣ���еľ��
																	&a,     //���͵�����
																0);	
			//break;
			}
		//vTaskDelay(300);
		
		}
	printf("choose�ĵڶ���ѭ������\r\n");	//����˵���ᵽ��
  Delay_ms(100);
}

/**********************************************************************
  * @ ������  �� LED_Task
  * @ ����˵���� LED_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void LED_Task(void* parameter)
{	
	  LCD_Init();
		LCD_String(55, 140,"by robin",16, YELLOW , BLACK);//����ǩ��
	  LCD_CHINSES_BYSJ(6,6);
		LCD_CHINSES_GetPassWord(20,30);
  while (1)
  {
		  //LCD_CHINSES_BYSJ(20,30);
		//10ms����һ��
//				LCD_Fill(1,   1 ,  8,   8,   WHITE);           // ��Ļ��䣬
//	LCD_Fill(121, 1 ,  129, 8,   RED);             // ͨ���Ŵ󾵹۲����Ͻǡ����Ͻ�����ɫ�飬�����������ص�X���꣬��߱��ڵ��˵�1�У��ұ߶���1��129
//	LCD_Fill(1,   152, 8  , 160, GREEN);
//	LCD_Fill(121, 152, 129, 160, BLUE);            // ͨ���۲��ϡ���ɫ�飬Y�����������������
//	LCD_String(6, 10,"test",24, YELLOW , BLACK);	
	//	printf("�������ڶ�\r\n");
		GetDistance();
		
  //LED1_ON;
  //  printf("LED_Task Running,LED1_ON\r\n");
  //  vTaskDelay(1000);   /* ��ʱ500��tick */
    
  //  LED1_OFF;     
   // printf("LED_Task Running,LED1_OFF\r\n");
    vTaskDelay(1000);   /* ��ʱ500��tick */
  }
}

/**********************************************************************
  * @ ������  �� KEY_Task
  * @ ����˵���� KEY_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void KEY_Task(void* parameter)
{	
	BaseType_t xReturn = pdTRUE;

  u16 a=0 , j=0 , t=0;
 while (1)
  {
 xReturn = xQueueReceive( key_Queue,
															&a,    //11
														portMAX_DELAY);
		
		if(a==10)
		{
			while(1)
		{
		//		key_val = key_scan();
				if(t==0)
				printf("�����������Ȱ�1���޸������밴2\r\n");
				t++;
				j=Matrix_Key_Scan();
		switch(j)
		{
			case 1:
				verify();                                    /*�Ա�����*/
				
			break;
				
			case 2:
				change_pwd();                                /*�޸�����*/
			 
			break;
		 
			default:
				/* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
				//OneNet_Publish(Tips, "ERROR:2");
				if (j > 0 && j < 10)		
				printf ("������Ϸ�ָ�\r\n\r\n");
			
			break;      
		}
			 
			Delay_ms(50);
		}
			
	}
				

		
		
		Delay_ms(50);

 // taskEXIT_CRITICAL(); 
   //vTaskDelay(1000);/* ��ʱ20��tick */
  }
  
}

/**********************************************************************
  * @ ������  �� AS608_Task
  * @ ����˵���� AS608_Task��������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  ********************************************************************/
static void AS608_Task(void* parameter)
{		
	BaseType_t xReturn = pdTRUE;
	//Delay_ms(1000);
	/*����STM32��ָ��ģ���ͨ��*/
  Connect_Test();
	u16 j=0 ;
	while(1)
	{
				xReturn = xQueueReceive( as608_Queue,
																	&j,    //22
																portMAX_DELAY);
		if(j==12)
		printf("ָ��ģ�����ڶ�\r\n");
		FR_Task();
		Delay_ms(300);
	}
 
}
/***********************************************************************
  * @ ������  �� BSP_Init
  * @ ����˵���� �弶�����ʼ�������а����ϵĳ�ʼ�����ɷ��������������
  * @ ����    ��   
  * @ ����ֵ  �� ��
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32�ж����ȼ�����Ϊ4����4bit��������ʾ��ռ���ȼ�����ΧΪ��0~15
	 * ���ȼ�����ֻ��Ҫ����һ�μ��ɣ��Ժ������������������Ҫ�õ��жϣ�
	 * ��ͳһ��������ȼ����飬ǧ��Ҫ�ٷ��飬�мɡ�
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED ��ʼ�� */
	LED_GPIO_Config();

	/* ���ڳ�ʼ��	*/
	USART_Config();
  
	System_Init_IO();
	
	System_Init();
	
  /* ������ʼ��	*/
  Key_GPIO_Config();
			//printf("1\r\n");
	  /*��ʼ�����λ�����*/
  rx_queue_init();
   
  /*��ʼ��ָ��ģ������*/
	AS608_Config();
			//printf("2\r\n");
	Matrix_ssKey_Pin_Init();
//			/*����STM32��ָ��ģ���ͨ��*/
 // Connect_Test();
 		//printf("over\r\n");
	

}

/********************************END OF FILE****************************/
