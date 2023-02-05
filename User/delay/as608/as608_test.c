/**
  ******************************************************************************
  * @file    bsp_as608.c
  * @author  fire
  * @version V1.0
  * @date    2013-xx-xx
  * @brief   ָ��ʶ��ģ��ʵ��
  ******************************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��STM32 F103-�Ե� ������  
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
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
//ȫ�ֱ���
//extern char *Tips ; //����
//Э���ļ�
//#include "onenet.h"
//#include "mqttkit.h"
//���
//#include "motor.h"

extern uint8_t TouchOut_int_flag;
uint8_t state;
 /**
  * @brief  ָ�����
  * @param  ��
  * @retval ��
  */
void Show_Message(void)
{
  AS608_INFO("   ������ָ�����ָ����������봰�ڰ��س������ٷ���ָ�����\r\n");
  AS608_INFO("   ָ��   ------      ���� \r\n");
  AS608_INFO("    1     ------    ���ָ�� \r\n");
  AS608_INFO("    2     ------    �Ա�ָ�� \r\n");
  AS608_INFO("    3     ------    ɾ��ָ���û�ָ��\r\n");
  AS608_INFO("    4     ------    ���ָ�ƿ�\r\n");  
}



/**
  * @brief  ָ��ģ�����
  * @param  ��
  * @retval ��
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
  //AS608_INFO("���յ��ַ���%d\r\n",ch);
  
  
  switch(ch)
  {
    case 1:
      Add_FR();                                    /*���ָ��*/
		  Show_Message();
    break;
			
		case 2:
      Compare_FR();                                /*�ȶ�ָ��*/
		  Show_Message();
    break;

    case 3:
      Del_FR();                                    /*ɾ��ָ���û�ָ��*/
		  Show_Message();
    break;
			
    case 4:
      Clean_FR();                                  /*���ָ�ƿ�*/
		  Show_Message();
    break;
   
    default:
      /* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
			//OneNet_Publish(Tips, "ERROR:2");
		if(ch>0&&ch<13)
		{
		  AS608_INFO("������Ϸ�ָ�\r\n\r\n");
      Show_Message();
		}
    break;      
  }
	}

  return;		
}
/**
  * @brief  MQTT����ָ��ģ��
  * @param  ��
  * @retval ��
  */
//void Mqtt_Task(char * Tips, int Cmd,uint16_t ID)
//{
//	printf("ID:%d\r\n",ID);
//  switch(Cmd)
//  {
//    case 1:
//      MQTT_Add_FR(Tips,ID);                                    /*���ָ��*/
//		  //Show_Message();
//    break;
//			
//		case 2:
//      Compare_FR_MQTT(Tips);                                /*�ȶ�ָ��*/
//		  //Show_Message();
//    break;

//    case 3:
//      Del_FR_MQTT(Tips,ID);                                    /*ɾ��ָ���û�ָ��*/
//		  //Show_Message();
//    break;
//			
//    case 4:
//      Clean_FR();                                  /*���ָ�ƿ�*/
//		  //Show_Message();
//    break;
//   
//    default:
//      /* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
//		  AS608_INFO("ERROR:2\r\n");
//		//	while(OneNet_Publish(Tips, "ERROR:2"));
//      //Show_Message();
//    break;      
//  }
//  return;		
//}

/**
  * @brief  �����ָ��ģ���ͨ��
  * @param  ��
  * @retval ��
  */
void  Connect_Test(void)
{   
   printf( "ָ��ģ��\r\n");
	
  if(PS_Connect(&AS608_Addr))                      /*��AS608����ͨ��*/
  {
		LED1_ON;		
    AS608_INFO("δ��⵽ָ��ģ�飬�������ӣ�����\r\n");			  
  }
  else
  {
		LED1_OFF;
		AS608_INFO("ͨѶ�ɹ�\r\n");
//		Show_Message();
  }
}


/**
  * @brief  ¼ָ��
  * @param  ��
  * @retval ��
  */
void  Add_FR(void)
{
  uint16_t  i,j,sure,ID;
 
  i=j=0;
	
  while(1)
  {
    switch (j)
    {
      case 0:                            /*ִ�е�1��*/
        
        i++;
      
        AS608_INFO("******����밴��ָ******\r\n");
        
        sure=PS_GetImage();              /*¼��ͼ��*/      
        if(sure == 0x00)
        {
          sure=PS_GenChar(CHAR_BUFFER1); /*��������1*/
          if(sure==0x00)
          { 
				    AS608_INFO("����ָ��1����������\r\n");
            
            sure=PS_HighSpeedSearch(CHAR_BUFFER1,0,PS_MAXNUM,&ID);
            if(sure==0x00)
            {
             AS608_INFO ("ָ���Ѵ��ڣ�ָ��ID��%d������\r\n\r\n",ID);
             
             return ;
            }              
            else 
            {
              i=0;
              j=1;                      /*��ת����2��*/
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
      
        AS608_INFO("******������ٰ�һ����ָ******\r\n");
      
        sure=PS_GetImage();
        if(sure==0x00)
        { 
          sure=PS_GenChar(CHAR_BUFFER2); /*��������2*/
          if(sure==0x00)
          { 
				    AS608_INFO("����ָ��2����������\r\n");
            i=0;
            j=2;                         /*��ת����3��*/
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
         
         AS608_INFO("���ڶԱ����������ָ��\r\n");
       
         sure=PS_Match();                /*��ȷ�ȶ���öָ������*/
         if(sure==0x00) 
         {
				   AS608_INFO("����ָ�ƶԱȳɹ�������\r\n");
           
           j=3;                         /*��ת����4��*/
         }
         else
         {
           AS608_INFO("�Ա�ʧ�ܣ�������¼��ָ�ƣ�����\r\n");
           ShowErrMessage(sure);
           i=0;
           j=0;
         }
        
       break;
         
         
       case 3:
         
         AS608_INFO("��������ָ��ģ��\r\n");
       
         sure=PS_RegModel();            /*�ϲ�����������ģ�壩*/
         if(sure==0x00) 	
         {
           AS608_INFO("����ָ��ģ��ɹ�������\r\n");
           j=4;                        /*��ת����5��*/
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
           AS608_INFO ("******���������洢ID����ΧΪ0��239******\r\n");
        
           ID=GET_NUM();
         }while(!(ID<PS_MAXNUM));
        
         sure=PS_StoreChar(CHAR_BUFFER2,ID);  /*����ģ��*/
         if(sure==0x00) 	
         {
           AS608_INFO("¼��ָ��ģ��ɹ�������\r\n\r\n");
      
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
      
      if(i==4)                       /*����4��û�а���ָ���˳�*/
      { 
        AS608_INFO("¼ָ��ʧ�ܣ�\r\n\r\n\r\n");

        break;
      }
  }
}
/**
  * @brief  ¼ָ��
  * @param  ��
  * @retval ��
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
//      case 0:                            /*ִ�е�1��*/
//        
//        i++;
//      
//        AS608_INFO("******����밴��ָ******\r\n");
//        while(OneNet_Publish(Tips, "TOK:1:0"));
//        sure=PS_GetImage();              /*¼��ͼ��*/      
//        if(sure == 0x00)
//        {
//          sure=PS_GenChar(CHAR_BUFFER1); /*��������1*/
//          if(sure==0x00)
//          { 
//				    AS608_INFO("����ָ��1����������\r\n");
//            
//            sure=PS_HighSpeedSearch(CHAR_BUFFER1,0,PS_MAXNUM,&ID);
//            if(sure==0x00)
//            {
//             AS608_INFO ("ָ���Ѵ��ڣ�ָ��ID��%d������\r\n\r\n",ID);
//						 sprintf(Num,"TF1:0[%d]",ID);
//						 while(OneNet_Publish(Tips, Num));
//             return ;
//            }              
//            else 
//            {
//              i=0;
//              j=1;                      /*��ת����2��*/
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
//        AS608_INFO("******������ٰ�һ����ָ******\r\n");
//				Delay_ms(100);
//				//while(OneNet_Publish(Tips, "TOK:1:1"));
//        sure=PS_GetImage();
//        if(sure==0x00)
//        { 
//          sure=PS_GenChar(CHAR_BUFFER2); /*��������2*/
//          if(sure==0x00)
//          { 
//				    AS608_INFO("����ָ��2����������\r\n");
//            i=0;
//            j=2;                         /*��ת����3��*/
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
//         AS608_INFO("���ڶԱ����������ָ��\r\n");
//       
//         sure=PS_Match();                /*��ȷ�ȶ���öָ������*/
//         if(sure==0x00) 
//         {
//				   AS608_INFO("����ָ�ƶԱȳɹ�������\r\n");
//           Delay_ms(100);
//				   //while(OneNet_Publish(Tips, "TOK:1:2")) ;
//           j=3;                         /*��ת����4��*/
//         }
//         else
//         {
//           AS608_INFO("�Ա�ʧ�ܣ�������¼��ָ�ƣ�����\r\n");
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
//         AS608_INFO("��������ָ��ģ��\r\n");
//       
//         sure=PS_RegModel();            /*�ϲ�����������ģ�壩*/
//         if(sure==0x00) 	
//         {
//           AS608_INFO("����ָ��ģ��ɹ�������\r\n");
//					 //while(OneNet_Publish(Tips, "TOK:1:3"));
//           j=4;                        /*��ת����5��*/
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
////           AS608_INFO ("******ID����ΧΪ0��239******\r\n");
////					 //AS608_SendData(ID);
////           //ID=GET_NUM();
////         }while(!(ID<PS_MAXNUM));
//				 if(id<PS_MAXNUM){
//						printf("ID:%d\r\n",id);
//						sure=PS_StoreChar(CHAR_BUFFER2,id);  /*����ģ��*/
//					  Delay_ms(100) ;
//				 }else{
//					 AS608_INFO("�ڴ治��") ;
//				 }
//				
//         if(sure==0x00) 	
//         {
//           AS608_INFO("¼��ָ��ģ��ɹ�������\r\n\r\n");
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
//      if(i==4)                       /*����4��û�а���ָ���˳�*/
//      { 
//        AS608_INFO("¼ָ��ʧ�ܣ�\r\n\r\n\r\n");
//				while(OneNet_Publish(Tips, "TF:1"));
//        break;
//      }
//  }
//}



/**
  * @brief  �ȶ�ָ�ƣ�ʶ��ָ�ƣ�
  * @param  ��
  * @retval ��
  */
void Compare_FR(void)  
{  
  uint16_t  ID=0;                    /*��ʼ��IDֵ*/  
  uint16_t  sure;
	
  AS608_INFO("******����Ա�ָ�ƣ��밴��ָ******\r\n");
	//Delay_ms(1000);
  sure=PS_GetImage();
  if(sure == 0x00)
  {
    sure=PS_GenChar(CHAR_BUFFER1);   /*��������1*/
    if(sure==0x00)
    {  
			/*��������ָ�ƿ�*/
      sure=PS_HighSpeedSearch(CHAR_BUFFER1,0,PS_MAXNUM,&ID);
      if(sure==0x00)
      {
        AS608_INFO ("�Ա�ָ�Ƴɹ���ָ��ID��%d������\r\n\r\n",ID);
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
  * @brief  �ȶ�ָ�ƣ�ʶ��ָ�ƣ�
  * @param  ��
  * @retval ��
  */
//void Compare_FR_MQTT(char * Tips)  
//{  
//  
//	uint16_t  ID=0;                    /*��ʼ��IDֵ*/  
//  uint16_t  sure;
//	char Str[100];
//  AS608_INFO("******����Ա�ָ�ƣ��밴��ָ******\r\n");
//	LED_BLUE;
//	Delay_ms(600);

//	//OneNet_Publish(Tips, "START");
//  sure=PS_GetImage();
//	
//	//while(OneNet_DevLink(Tips))			//����OneNETile(OneNet_DevLink(Tips))			//����OneNET
//  if(sure == 0x00)
//  {
//		
//    sure=PS_GenChar(CHAR_BUFFER1);   /*��������1*/

//    if(sure==0x00)
//    {  
//			/*��������ָ�ƿ�*/
//      sure=PS_HighSpeedSearch(CHAR_BUFFER1,0,PS_MAXNUM,&ID);
//			
//      if(sure==0x00)
//      {
//        AS608_INFO ("�Ա�ָ�Ƴɹ���ָ��ID��%d������\r\n\r\n",ID);
//				sprintf(Str,"TOK:2:[%d]",ID);
//				state = 1;
//				//Door(Tips,1);
//				LED_GREEN;
//				while(OneNet_Publish(Tips, Str));
//				
//      }
//      else 
//      {
//				printf("����Чָ��\r\n");
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
//			printf("����Чָ��\r\n");
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
//			printf("����Чָ��\r\n");
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
//      /* �������ָ��ָ���ַ�����ӡ��ʾ��Ϣ */
//		  while(OneNet_Publish(Tips,"ERROR:2"));
//    break;		
// }
//}
///**
//  * @brief  ���ָ�ƿ�
//  * @param  ��
//  * @retval ��
//  */
void Clean_FR(void) 
{
  uint16_t  sure;
  
	AS608_INFO("���ָ�ƿ�\r\n");
  
	sure=PS_Empty();     
	if(sure == 0x00)
	{
		AS608_INFO("���ָ�ƿ�ɹ�������\r\n\r\n");
		//while(OneNet_Publish(Tips, "TOK:4"));
	}
	else 
	{
		ShowErrMessage(sure);
	}

}


/**
  * @brief  ɾ��ָ���û�ָ��
  * @param  ��
  * @retval ��
  */
void Del_FR(void)
{ 
	uint32_t  ID;
	uint16_t  sure;
  
	AS608_INFO("ɾ��ָ���û�ID\r\n");
  
	do
	{ 
		AS608_INFO ("******���������洢ID����ΧΪ0��239******\r\n");
		ID=GET_NUM();
	}while(!(ID<PS_MAXNUM));         /*����ID���ܳ���ָ��ģ���������*/
	
	sure=PS_DeletChar(ID,1);         /*ɾ��ָ���û���ָ��ģ��*/
	
	if(sure == 0x00)                
	{
		AS608_INFO("ɾ��ָ���û�ָ�Ƴɹ�������\r\n\r\n");
	}
	else 
	{
		ShowErrMessage(sure);
	}
}
 /**
  * @brief  ɾ��ָ���û�ָ��
  * @param  ��
  * @retval ��
  */
//void Del_FR_MQTT(char *Tips , uint16_t id)
//{ 
//	uint32_t  ID;
//	uint16_t  sure;
//  ID =id;
//	char str[10];
//	AS608_INFO("ɾ��ָ���û�ID\r\n");
//  
////	do
////	{ 
////		AS608_INFO ("******����洢ID����ΧΪ0��239******\r\n");
////		ID=GET_NUM();
////	}while(!(ID<PS_MAXNUM));         /*����ID���ܳ���ָ��ģ���������*/
//	if(ID<PS_MAXNUM){
//		sure=PS_DeletChar(ID,1);         /*ɾ��ָ���û���ָ��ģ��*/
//	}

//	if(sure == 0x00)                
//	{
//		sprintf(str,"TOK:3:[%d]",id);
//		while(OneNet_Publish(Tips, str));
//		AS608_INFO("ɾ��ָ���û�ָ�Ƴɹ�������\r\n\r\n");
//	}
//	else 
//	{
//		ShowErrMessage(sure);
//	}
//}

///*********************************************END OF FILE**********************/
