/**
  ******************************************************************************
  * @file    bsp_as608.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   指纹识别模块实验
  ******************************************************************************
  * @attention
  *
  * 实验平台:野火STM32 F103-霸道 开发板  
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************************
  */ 

#include "./as608/as608_test.h"
#include "./as608/bsp_as608.h"
#include "RT_Delay.h"
#include  "delay.h"
#include "matrix_key.h"

//#include "RT_Delay.h"
//#include "bsp_SysTick.h"
#include "./led/bsp_led.h"
//全局变量
//extern char *Tips ; //主题
//协议文件
//#include "onenet.h"
//#include "mqttkit.h"
//电机
//#include "motor.h"

extern uint8_t TouchOut_int_flag;
uint8_t state;
 /**
  * @brief  指令解析
  * @param  无
  * @retval 无
  */
void Show_Message(void)
{
  AS608_INFO("   请输入指令，输入指令后请在输入窗口按回车键后再发送指令！！！\r\n");
  AS608_INFO("   指令   ------      功能 \r\n");
  AS608_INFO("    1     ------    添加指纹 \r\n");
  AS608_INFO("    2     ------    对比指纹 \r\n");
  AS608_INFO("    3     ------    删除指定用户指纹\r\n");
  AS608_INFO("    4     ------    清空指纹库\r\n");  
}



/**
  * @brief  指纹模块操作
  * @param  无
  * @retval 无
  */
void  FR_Task(void)
{	
  uint32_t   ch;
	Show_Message();
	while(1)
	{
	ch = Matrix_Key_Scan() ;	
		
	if(ch>0&&ch<13)
	printf("%d\r\n",ch);
  //AS608_INFO("接收到字符：%d\r\n",ch);
  
  
  switch(ch)
  {
    case 1:
      Add_FR();                                    /*添加指纹*/
		  Show_Message();
    break;
			
		case 2:
      Compare_FR();                                /*比对指纹*/
		  Show_Message();
    break;

    case 3:
      Del_FR();                                    /*删除指定用户指纹*/
		  Show_Message();
    break;
			
    case 4:
      Clean_FR();                                  /*清空指纹库*/
		  Show_Message();
    break;
   
    default:
      /* 如果不是指定指令字符，打印提示信息 */
			//OneNet_Publish(Tips, "ERROR:2");
		if(ch>0&&ch<13)
		{
		  AS608_INFO("请输入合法指令！\r\n\r\n");
      Show_Message();
		}
    break;      
  }
	}

  return;		
}
/**
  * @brief  MQTT操作指纹模块
  * @param  无
  * @retval 无
  */
//void Mqtt_Task(char * Tips, int Cmd,uint16_t ID)
//{
//	printf("ID:%d\r\n",ID);
//  switch(Cmd)
//  {
//    case 1:
//      MQTT_Add_FR(Tips,ID);                                    /*添加指纹*/
//		  //Show_Message();
//    break;
//			
//		case 2:
//      Compare_FR_MQTT(Tips);                                /*比对指纹*/
//		  //Show_Message();
//    break;

//    case 3:
//      Del_FR_MQTT(Tips,ID);                                    /*删除指定用户指纹*/
//		  //Show_Message();
//    break;
//			
//    case 4:
//      Clean_FR();                                  /*清空指纹库*/
//		  //Show_Message();
//    break;
//   
//    default:
//      /* 如果不是指定指令字符，打印提示信息 */
//		  AS608_INFO("ERROR:2\r\n");
//		//	while(OneNet_Publish(Tips, "ERROR:2"));
//      //Show_Message();
//    break;      
//  }
//  return;		
//}

/**
  * @brief  检测与指纹模块的通信
  * @param  无
  * @retval 无
  */
void  Connect_Test(void)
{   
   printf( "指纹模块\r\n");
	
  if(PS_Connect(&AS608_Addr))                      /*与AS608串口通信*/
  {
		LED1_ON;		
    AS608_INFO("未检测到指纹模块，请检查连接！！！\r\n");			  
  }
  else
  {
		LED1_OFF;
		AS608_INFO("通讯成功\r\n");
//		Show_Message();
  }
}


/**
  * @brief  录指纹
  * @param  无
  * @retval 无
  */
void  Add_FR(void)
{
  uint16_t  i,j,sure,ID;
 
  i=j=0;
	
  while(1)
  {
    switch (j)
    {
      case 0:                            /*执行第1步*/
        
        i++;
      
        AS608_INFO("******命令：请按手指******\r\n");
        
        sure=PS_GetImage();              /*录入图像*/      
        if(sure == 0x00)
        {
          sure=PS_GenChar(CHAR_BUFFER1); /*生成特征1*/
          if(sure==0x00)
          { 
				    AS608_INFO("输入指纹1正常！！！\r\n");
            
            sure=PS_HighSpeedSearch(CHAR_BUFFER1,0,PS_MAXNUM,&ID);
            if(sure==0x00)
            {
             AS608_INFO ("指纹已存在，指纹ID：%d！！！\r\n\r\n",ID);
             
             return ;
            }              
            else 
            {
              i=0;
              j=1;                      /*跳转到第2步*/
             }
           }
				 else
				 {
					 ShowErrMessage(sure);	       
				 }
        }
			  else 
			  {
				  ShowErrMessage(sure);	
			  }
      break;


      case 1:
        
        i++;
      
        AS608_INFO("******命令：请再按一次手指******\r\n");
      
        sure=PS_GetImage();
        if(sure==0x00)
        { 
          sure=PS_GenChar(CHAR_BUFFER2); /*生成特征2*/
          if(sure==0x00)
          { 
				    AS608_INFO("输入指纹2正常！！！\r\n");
            i=0;
            j=2;                         /*跳转到第3步*/
          }
				  else 
				  {
					  ShowErrMessage(sure);
				  }

         }
			   else 
			   {
			   ShowErrMessage(sure);
         }				
       break;

			
       case 2:
         
         AS608_INFO("正在对比两次输入的指纹\r\n");
       
         sure=PS_Match();                /*精确比对两枚指纹特征*/
         if(sure==0x00) 
         {
				   AS608_INFO("输入指纹对比成功！！！\r\n");
           
           j=3;                         /*跳转到第4步*/
         }
         else
         {
           AS608_INFO("对比失败，请重新录入指纹！！！\r\n");
           ShowErrMessage(sure);
           i=0;
           j=0;
         }
        
       break;
         
         
       case 3:
         
         AS608_INFO("正在生成指纹模块\r\n");
       
         sure=PS_RegModel();            /*合并特征（生成模板）*/
         if(sure==0x00) 	
         {
           AS608_INFO("生成指纹模块成功！！！\r\n");
           j=4;                        /*跳转到第5步*/
         } 
         else 
        {
         j =0;
         ShowErrMessage(sure);
        }
        
       break;
        

       case 4:
        
         do
         { 
           AS608_INFO ("******命令：请输入存储ID，范围为0—239******\r\n");
        
           ID=GET_NUM();
         }while(!(ID<PS_MAXNUM));
        
         sure=PS_StoreChar(CHAR_BUFFER2,ID);  /*储存模板*/
         if(sure==0x00) 	
         {
           AS608_INFO("录入指纹模块成功！！！\r\n\r\n");
      
           return ;
         } 
         else 
         {
           j =0;
           ShowErrMessage(sure);
         }
       break;
      }
    
      delay_ms(1000);
      
      if(i==4)                       /*超过4次没有按手指则退出*/
      { 
        AS608_INFO("录指纹失败！\r\n\r\n\r\n");

        break;
      }
  }
}
/**
  * @brief  录指纹
  * @param  无
  * @retval 无
  */
//void  MQTT_Add_FR(char *Tips , uint16_t ID)
//{
//  uint16_t  i,j,sure,id;
// 
//  i=j=0;
//	printf("ID:%d\r\n",ID);
//	id = ID;
//	char Num[20]; 
//  while(1)
//  {
//    switch (j)
//    {
//      case 0:                            /*执行第1步*/
//        
//        i++;
//      
//        AS608_INFO("******命令：请按手指******\r\n");
//        while(OneNet_Publish(Tips, "TOK:1:0"));
//        sure=PS_GetImage();              /*录入图像*/      
//        if(sure == 0x00)
//        {
//          sure=PS_GenChar(CHAR_BUFFER1); /*生成特征1*/
//          if(sure==0x00)
//          { 
//				    AS608_INFO("输入指纹1正常！！！\r\n");
//            
//            sure=PS_HighSpeedSearch(CHAR_BUFFER1,0,PS_MAXNUM,&ID);
//            if(sure==0x00)
//            {
//             AS608_INFO ("指纹已存在，指纹ID：%d！！！\r\n\r\n",ID);
//						 sprintf(Num,"TF1:0[%d]",ID);
//						 while(OneNet_Publish(Tips, Num));
//             return ;
//            }              
//            else 
//            {
//              i=0;
//              j=1;                      /*跳转到第2步*/
//             }
//           }
//				 else
//				 {
//					 ShowErrMessage(sure);	       
//				 }
//        }
//			  else 
//			  {
//				  ShowErrMessage(sure);	
//			  }
//      break;


//      case 1:
//        
//        i++;
//      
//        AS608_INFO("******命令：请再按一次手指******\r\n");
//				Delay_ms(100);
//				//while(OneNet_Publish(Tips, "TOK:1:1"));
//        sure=PS_GetImage();
//        if(sure==0x00)
//        { 
//          sure=PS_GenChar(CHAR_BUFFER2); /*生成特征2*/
//          if(sure==0x00)
//          { 
//				    AS608_INFO("输入指纹2正常！！！\r\n");
//            i=0;
//            j=2;                         /*跳转到第3步*/
//          }
//				  else 
//				  {
//					  ShowErrMessage(sure);
//				  }

//         }
//			   else 
//			   {
//			   ShowErrMessage(sure);
//         }				
//       break;

//			
//       case 2:
//         
//         AS608_INFO("正在对比两次输入的指纹\r\n");
//       
//         sure=PS_Match();                /*精确比对两枚指纹特征*/
//         if(sure==0x00) 
//         {
//				   AS608_INFO("输入指纹对比成功！！！\r\n");
//           Delay_ms(100);
//				   //while(OneNet_Publish(Tips, "TOK:1:2")) ;
//           j=3;                         /*跳转到第4步*/
//         }
//         else
//         {
//           AS608_INFO("对比失败，请重新录入指纹！！！\r\n");
//           ShowErrMessage(sure);
//					 //while(OneNet_Publish(Tips, "TF:1:2"))
//           i=0;
//           j=0;
//         }
//        
//       break;
//         
//         
//       case 3:
//         
//         AS608_INFO("正在生成指纹模块\r\n");
//       
//         sure=PS_RegModel();            /*合并特征（生成模板）*/
//         if(sure==0x00) 	
//         {
//           AS608_INFO("生成指纹模块成功！！！\r\n");
//					 //while(OneNet_Publish(Tips, "TOK:1:3"));
//           j=4;                        /*跳转到第5步*/
//         } 
//         else 
//        {
//         j =0;
//         ShowErrMessage(sure);
//        }
//        
//       break;
//        

//       case 4:
//        
////         do
////         { 
////           AS608_INFO ("******ID，范围为0—239******\r\n");
////					 //AS608_SendData(ID);
////           //ID=GET_NUM();
////         }while(!(ID<PS_MAXNUM));
//				 if(id<PS_MAXNUM){
//						printf("ID:%d\r\n",id);
//						sure=PS_StoreChar(CHAR_BUFFER2,id);  /*储存模板*/
//					  Delay_ms(100) ;
//				 }else{
//					 AS608_INFO("内存不足") ;
//				 }
//				
//         if(sure==0x00) 	
//         {
//           AS608_INFO("录入指纹模块成功！！！\r\n\r\n");
//					 Delay_ms(100);
//				   while(OneNet_Publish(Tips, "TOK:1"));
//           return ;
//         } 
//         else 
//         {
//           j =0;
//           ShowErrMessage(sure);
//         }
//       break;
//      }
//    
//      Delay_ms(1000);
//      
//      if(i==4)                       /*超过4次没有按手指则退出*/
//      { 
//        AS608_INFO("录指纹失败！\r\n\r\n\r\n");
//				while(OneNet_Publish(Tips, "TF:1"));
//        break;
//      }
//  }
//}



/**
  * @brief  比对指纹（识别指纹）
  * @param  无
  * @retval 无
  */
void Compare_FR(void)  
{  
  uint16_t  ID=0;                    /*初始化ID值*/  
  uint16_t  sure;
	
  AS608_INFO("******命令：对比指纹，请按手指******\r\n");
	//Delay_ms(1000);
  sure=PS_GetImage();
  if(sure == 0x00)
  {
    sure=PS_GenChar(CHAR_BUFFER1);   /*生成特征1*/
    if(sure==0x00)
    {  
			/*高速搜索指纹库*/
      sure=PS_HighSpeedSearch(CHAR_BUFFER1,0,PS_MAXNUM,&ID);
      if(sure==0x00)
      {
        AS608_INFO ("对比指纹成功，指纹ID：%d！！！\r\n\r\n",ID);
      }
      else 
      {
			ShowErrMessage(sure);
			}
    }
    else 
    {   
      ShowErrMessage(sure);	
    }
  }
	else  
	{
	  ShowErrMessage(sure);
	}
 
}

 /**
  * @brief  比对指纹（识别指纹）
  * @param  无
  * @retval 无
  */
//void Compare_FR_MQTT(char * Tips)  
//{  
//  
//	uint16_t  ID=0;                    /*初始化ID值*/  
//  uint16_t  sure;
//	char Str[100];
//  AS608_INFO("******命令：对比指纹，请按手指******\r\n");
//	LED_BLUE;
//	Delay_ms(600);

//	//OneNet_Publish(Tips, "START");
//  sure=PS_GetImage();
//	
//	//while(OneNet_DevLink(Tips))			//接入OneNETile(OneNet_DevLink(Tips))			//接入OneNET
//  if(sure == 0x00)
//  {
//		
//    sure=PS_GenChar(CHAR_BUFFER1);   /*生成特征1*/

//    if(sure==0x00)
//    {  
//			/*高速搜索指纹库*/
//      sure=PS_HighSpeedSearch(CHAR_BUFFER1,0,PS_MAXNUM,&ID);
//			
//      if(sure==0x00)
//      {
//        AS608_INFO ("对比指纹成功，指纹ID：%d！！！\r\n\r\n",ID);
//				sprintf(Str,"TOK:2:[%d]",ID);
//				state = 1;
//				//Door(Tips,1);
//				LED_GREEN;
//				while(OneNet_Publish(Tips, Str));
//				
//      }
//      else 
//      {
//				printf("无有效指纹\r\n");
//				LED_RED;
//				Delay_ms(300);
//				LED_RGBOFF;
//				Delay_ms(500) ;
//				LED_GREEN;
//				while(OneNet_Publish(Tips, "ERROR:1"));
//			}
//    }
//    else                  
//    {
//			printf("无有效指纹\r\n");
//	LED_RED;
//				Delay_ms(300);
//				LED_RGBOFF;
//				Delay_ms(500) ;
//				LED_GREEN;
//			while(OneNet_Publish(Tips, "ERROR:1"));			
//    }
//  }
//	else  
//	{
//			printf("无有效指纹\r\n");
//			LED1_ON;
//			Delay_ms(500);
//			LED1_OFF
//			Delay_ms(500) ;
//			LED_GREEN;
//			while(OneNet_Publish(Tips, "ERROR:1"));
//	}
// 
//}
//void Door_ID_Open(char *Tips,int State){
//	 
//	switch(State) {
//		case 1:
//				 Door(Tips,1);
//				 state = 0;
//			break;
//		
//   default:
//      /* 如果不是指定指令字符，打印提示信息 */
//		  while(OneNet_Publish(Tips,"ERROR:2"));
//    break;		
// }
//}
///**
//  * @brief  清空指纹库
//  * @param  无
//  * @retval 无
//  */
void Clean_FR(void) 
{
  uint16_t  sure;
  
	AS608_INFO("清空指纹库\r\n");
  
	sure=PS_Empty();     
	if(sure == 0x00)
	{
		AS608_INFO("清空指纹库成功！！！\r\n\r\n");
		//while(OneNet_Publish(Tips, "TOK:4"));
	}
	else 
	{
		ShowErrMessage(sure);
	}

}


/**
  * @brief  删除指定用户指纹
  * @param  无
  * @retval 无
  */
void Del_FR(void)
{ 
	uint32_t  ID;
	uint16_t  sure;
  
	AS608_INFO("删除指定用户ID\r\n");
  
	do
	{ 
		AS608_INFO ("******命令：请输入存储ID，范围为0—239******\r\n");
		ID=GET_NUM();
	}while(!(ID<PS_MAXNUM));         /*输入ID不能超过指纹模块最大容量*/
	
	sure=PS_DeletChar(ID,1);         /*删除指定用户的指纹模板*/
	
	if(sure == 0x00)                
	{
		AS608_INFO("删除指定用户指纹成功！！！\r\n\r\n");
	}
	else 
	{
		ShowErrMessage(sure);
	}
}
 /**
  * @brief  删除指定用户指纹
  * @param  无
  * @retval 无
  */
//void Del_FR_MQTT(char *Tips , uint16_t id)
//{ 
//	uint32_t  ID;
//	uint16_t  sure;
//  ID =id;
//	char str[10];
//	AS608_INFO("删除指定用户ID\r\n");
//  
////	do
////	{ 
////		AS608_INFO ("******命令：存储ID，范围为0—239******\r\n");
////		ID=GET_NUM();
////	}while(!(ID<PS_MAXNUM));         /*输入ID不能超过指纹模块最大容量*/
//	if(ID<PS_MAXNUM){
//		sure=PS_DeletChar(ID,1);         /*删除指定用户的指纹模板*/
//	}

//	if(sure == 0x00)                
//	{
//		sprintf(str,"TOK:3:[%d]",id);
//		while(OneNet_Publish(Tips, str));
//		AS608_INFO("删除指定用户指纹成功！！！\r\n\r\n");
//	}
//	else 
//	{
//		ShowErrMessage(sure);
//	}
//}

///*********************************************END OF FILE**********************/
