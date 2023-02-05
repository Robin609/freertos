/*
*************************************************************************
*                             包含的头文件
*************************************************************************
*/ 
/* FreeRTOS头文件 */
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"//事件标志组
#include "semphr.h "
#include "queue.h"
/* 开发板硬件bsp头文件 */
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
/**************************** 任务句柄 ********************************/
/* 
 * 任务句柄是一个指针，用于指向一个任务，当任务创建好之后，它就具有了一个任务句柄
 * 以后我们要想操作这个任务都需要通过这个任务句柄，如果是自身的任务操作自己，那么
 * 这个句柄可以为NULL。
 */
static TaskHandle_t AppTaskCreate_Handle = NULL;/* 创建任务句柄 */
static TaskHandle_t LED_Task_Handle = NULL;/* LED任务句柄 */
static TaskHandle_t KEY_Task_Handle = NULL;/* KEY任务句柄 */
static TaskHandle_t AS608_Task_Handle = NULL;/* AS608任务句柄 */
static TaskHandle_t CHOOSE_Task_Handle = NULL;/* AS608任务句柄 */

/********************************** 内核对象句柄 *********************************/
/*
 * 信号量，消息队列，事件标志组，软件定时器这些都属于内核的对象，要想使用这些内核
 * 对象，必须先创建，创建成功之后会返回一个相应的句柄。实际上就是一个指针，后续我
 * 们就可以通过这个句柄操作这些内核对象。
 *
 * 内核对象说白了就是一种全局的数据结构，通过这些数据结构我们可以实现任务间的通信，
 * 任务间的事件同步等各种功能。至于这些功能的实现我们是通过调用这些内核对象的函数
 * 来完成的
 * 
 */
 
SemaphoreHandle_t keyboard_Handle;
QueueHandle_t as608_Queue =NULL;
QueueHandle_t key_Queue =NULL;



/******************************* 全局变量声明 ************************************/
/*
 * 当我们在写应用程序的时候，可能需要用到一些全局变量。
 */
u8 i = 0;
u8 Passwd[6] = {1,2,3,4,5,6};
u8 ascci_Passwd[6] = {49,50,51,52,53,54};
u8 Input_pwd[6] = {0};
u8 buf[4] = {0x00,0x01,0x0B,0x11};


/***********宏定义**********/
#define  QUEUE_LEN 4
#define  QUEUE_SIZE 4

/*
*************************************************************************
*                             函数声明
*************************************************************************
*/                                                                                                                                                      
static void AppTaskCreate(void);/* 用于创建任务 */

static void LED_Task(void* pvParameters);/* LED_Task任务实现 */
static void KEY_Task(void* pvParameters);/* KEY_Task任务实现 */
static void AS608_Task(void* pvParameters);/* AS608_Task任务实现 */
static void CHOOSE_Task(void* pvParameters);/* AS608_Task任务实现 */


static void BSP_Init(void);/* 用于初始化板载相关资源 */

/*****************************************************************
  * @brief  主函数
  * @param  无
  * @retval 无
  * @note   第一步：开发板硬件初始化 
            第二步：创建APP应用任务
            第三步：启动FreeRTOS，开始多任务调度
  ****************************************************************/
int main(void)
{	
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  /* 开发板硬件初始化 */
  BSP_Init();

  printf("这是一个FreeRTOS任务测试实验！\n");
  
//		LCD_Fill(1,   1 ,  8,   8,   WHITE);           // 屏幕填充，
//	LCD_Fill(121, 1 ,  129, 8,   RED);             // 通过放大镜观察左上角、右上角两个色块，发现物理像素的X坐标，左边被遮挡了第1列，右边多了1列129
//	LCD_Fill(1,   152, 8  , 160, GREEN);
//	LCD_Fill(121, 152, 129, 160, BLUE);            // 通过观察上、下色块，Y坐标与物理像素相符
//	LCD_String(6, 10,"test",24, YELLOW , BLACK);	
   /* 创建AppTaskCreate任务 */
  xReturn = xTaskCreate((TaskFunction_t )AppTaskCreate,  /* 任务入口函数 */
                        (const char*    )"AppTaskCreate",/* 任务名字 */
                        (uint16_t       )512,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )1, /* 任务的优先级 */
                        (TaskHandle_t*  )&AppTaskCreate_Handle);/* 任务控制块指针 */ 
  /* 启动任务调度 */           
  if(pdPASS == xReturn)
    vTaskStartScheduler();   /* 启动任务，开启调度 */
    return -1;  
  
  while(1);   /* 正常不会执行到这里 */    
}


/***********************************************************************
  * @ 函数名  ： AppTaskCreate
  * @ 功能说明： 为了方便管理，所有的任务创建函数都放在这个函数里面
  * @ 参数    ： 无  
  * @ 返回值  ： 无
  **********************************************************************/
static void AppTaskCreate(void)
{
  BaseType_t xReturn = pdPASS;/* 定义一个创建信息返回值，默认为pdPASS */
  
  taskENTER_CRITICAL();           //进入临界区
  
		/* 创建 as608_Queue */
	  as608_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* 消息队列的长度 */
														(UBaseType_t ) QUEUE_SIZE);/* 消息的大小?
,→*/
  if (NULL != as608_Queue)
		printf(" 创建 as608_Queue 消息队列成功!\r\n");
	
		/* 创建 key_Queue */
	  key_Queue = xQueueCreate((UBaseType_t ) QUEUE_LEN,/* 消息队列的长度 */
														(UBaseType_t ) QUEUE_SIZE);/* 消息的大小?
,→*/
  if (NULL != key_Queue)
		printf(" 创建 key_Queue 消息队列成功!\r\n");
	
  /* 创建LED_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )LED_Task, /* 任务入口函数 */
                        (const char*    )"LED_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )3,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&LED_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建LED_Task任务成功!\r\n");
	
  /* 创建KEY_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )KEY_Task,  /* 任务入口函数 */
                        (const char*    )"KEY_Task",/* 任务名字 */
                        (uint16_t       )1024,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )3, /* 任务的优先级 */
                        (TaskHandle_t*  )&KEY_Task_Handle);/* 任务控制块指针 */ 
  if(pdPASS == xReturn)
    printf("创建KEY_Task任务成功!\r\n");
	
  /* 创建AS608_Task任务 */
  xReturn = xTaskCreate((TaskFunction_t )AS608_Task,  /* 任务入口函数 */
                        (const char*    )"AS608_Task",/* 任务名字 */
                        (uint16_t       )1024,  /* 任务栈大小 */
                        (void*          )NULL,/* 任务入口函数参数 */
                        (UBaseType_t    )4, /* 任务的优先级 */
                        (TaskHandle_t*  )&AS608_Task_Handle);/* 任务控制块指针 */ 
  if(pdPASS == xReturn)
    printf("创建AS608_Task任务成功!\r\n");  
	
	/*创建CHOOSE_task任务*/
	  xReturn = xTaskCreate((TaskFunction_t )CHOOSE_Task, /* 任务入口函数 */
                        (const char*    )"LED_Task",/* 任务名字 */
                        (uint16_t       )512,   /* 任务栈大小 */
                        (void*          )NULL,	/* 任务入口函数参数 */
                        (UBaseType_t    )2,	    /* 任务的优先级 */
                        (TaskHandle_t*  )&CHOOSE_Task_Handle);/* 任务控制块指针 */
  if(pdPASS == xReturn)
    printf("创建CHOOSE_Task任务成功!\r\n");
	
	
  vTaskDelete(AppTaskCreate_Handle); //删除AppTaskCreate任务
  
  taskEXIT_CRITICAL();            //退出临界区
}


/**********************************************************************
  * @ 函数名  ： CHOOSE_Task
  * @ 功能说明： CHOOSE_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void CHOOSE_Task(void* parameter)			
{	
	BaseType_t xReturn = pdTRUE;
	uint32_t a = 0;
	//a=Matrix_Key_Scan();
 u32 q =0;

//			/*测试STM32与指纹模块的通信*/
//  Connect_Test();

	
	//LCD_CHINSES_GetPassWord(20,30);
		while (1)
		{
//		LCD_String(55, 140,"by robin",16, YELLOW , BLACK);//个人签名
//	  LCD_CHINSES_BYSJ(6,6);
//		LCD_CHINSES_GetPassWord(20,30);
			if(q==0)
			printf("请开始按键\r\n");
			q++;
			
			a = Matrix_Key_Scan();
//		key_val = key_scan();
		if (a == 10)
			{
			printf("you choose is keyborad\r\n");
			xReturn = xQueueSend( key_Queue, //消息队列的句柄
																	&a,     //发送的内容
																0);	
			//break;
			}
		if (a == 12)
			{
			printf("you choose is fingerprint\r\n");
			xReturn = xQueueSend( as608_Queue, //消息队列的句柄
																	&a,     //发送的内容
																0);	
			//break;
			}
		//vTaskDelay(300);
		
		}
	printf("choose的第二个循环结束\r\n");	//按理说不会到这
  Delay_ms(100);
}

/**********************************************************************
  * @ 函数名  ： LED_Task
  * @ 功能说明： LED_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void LED_Task(void* parameter)
{	
	  LCD_Init();
		LCD_String(55, 140,"by robin",16, YELLOW , BLACK);//个人签名
	  LCD_CHINSES_BYSJ(6,6);
		LCD_CHINSES_GetPassWord(20,30);
  while (1)
  {
		  //LCD_CHINSES_BYSJ(20,30);
		//10ms运行一次
//				LCD_Fill(1,   1 ,  8,   8,   WHITE);           // 屏幕填充，
//	LCD_Fill(121, 1 ,  129, 8,   RED);             // 通过放大镜观察左上角、右上角两个色块，发现物理像素的X坐标，左边被遮挡了第1列，右边多了1列129
//	LCD_Fill(1,   152, 8  , 160, GREEN);
//	LCD_Fill(121, 152, 129, 160, BLUE);            // 通过观察上、下色块，Y坐标与物理像素相符
//	LCD_String(6, 10,"test",24, YELLOW , BLACK);	
	//	printf("超声波在动\r\n");
		GetDistance();
		
  //LED1_ON;
  //  printf("LED_Task Running,LED1_ON\r\n");
  //  vTaskDelay(1000);   /* 延时500个tick */
    
  //  LED1_OFF;     
   // printf("LED_Task Running,LED1_OFF\r\n");
    vTaskDelay(1000);   /* 延时500个tick */
  }
}

/**********************************************************************
  * @ 函数名  ： KEY_Task
  * @ 功能说明： KEY_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
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
				printf("输入密码请先按1，修改密码请按2\r\n");
				t++;
				j=Matrix_Key_Scan();
		switch(j)
		{
			case 1:
				verify();                                    /*对比密码*/
				
			break;
				
			case 2:
				change_pwd();                                /*修改密码*/
			 
			break;
		 
			default:
				/* 如果不是指定指令字符，打印提示信息 */
				//OneNet_Publish(Tips, "ERROR:2");
				if (j > 0 && j < 10)		
				printf ("请输入合法指令！\r\n\r\n");
			
			break;      
		}
			 
			Delay_ms(50);
		}
			
	}
				

		
		
		Delay_ms(50);

 // taskEXIT_CRITICAL(); 
   //vTaskDelay(1000);/* 延时20个tick */
  }
  
}

/**********************************************************************
  * @ 函数名  ： AS608_Task
  * @ 功能说明： AS608_Task任务主体
  * @ 参数    ：   
  * @ 返回值  ： 无
  ********************************************************************/
static void AS608_Task(void* parameter)
{		
	BaseType_t xReturn = pdTRUE;
	//Delay_ms(1000);
	/*测试STM32与指纹模块的通信*/
  Connect_Test();
	u16 j=0 ;
	while(1)
	{
				xReturn = xQueueReceive( as608_Queue,
																	&j,    //22
																portMAX_DELAY);
		if(j==12)
		printf("指纹模块有在动\r\n");
		FR_Task();
		Delay_ms(300);
	}
 
}
/***********************************************************************
  * @ 函数名  ： BSP_Init
  * @ 功能说明： 板级外设初始化，所有板子上的初始化均可放在这个函数里面
  * @ 参数    ：   
  * @ 返回值  ： 无
  *********************************************************************/
static void BSP_Init(void)
{
	/*
	 * STM32中断优先级分组为4，即4bit都用来表示抢占优先级，范围为：0~15
	 * 优先级分组只需要分组一次即可，以后如果有其他的任务需要用到中断，
	 * 都统一用这个优先级分组，千万不要再分组，切忌。
	 */
	NVIC_PriorityGroupConfig( NVIC_PriorityGroup_4 );
	
	/* LED 初始化 */
	LED_GPIO_Config();

	/* 串口初始化	*/
	USART_Config();
  
	System_Init_IO();
	
	System_Init();
	
  /* 按键初始化	*/
  Key_GPIO_Config();
			//printf("1\r\n");
	  /*初始化环形缓冲区*/
  rx_queue_init();
   
  /*初始化指纹模块配置*/
	AS608_Config();
			//printf("2\r\n");
	Matrix_ssKey_Pin_Init();
//			/*测试STM32与指纹模块的通信*/
 // Connect_Test();
 		//printf("over\r\n");
	

}

/********************************END OF FILE****************************/
