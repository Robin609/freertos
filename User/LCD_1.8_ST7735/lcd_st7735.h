#ifndef __LCD_H
#define __LCD_H		
/***********************************************************************************************************************************
    *	@file  	  w25qxx.c
	*   @date     2020-08-17		
	*	@author   ħŮ������   ����
    *   @brief    �������     QȺ�ļ��У�262901124  ��������
    *   @taobao   �Ա�Ӳ��     ttps://demoboard.taobao.com/
    ********************************************************************************************************************************
    ** 1.8���ɫ��ʾ��ģ������˵��(ST7735)
    ** GND         ��Դ��
    ** VCC         3.3V����Ҫ��5V, ģ�鲻��LDO��5V��������
    ** SCL/SCK     SPI��SCK����  
    ** SDA/DIN     SPI��MOSI���� 
    ** RES/RST     ��λ���ţ���ú͵�Ƭ���ĸ�λ�������
    ** DC /RS      �������ѡ������
    ** CS /CE      SPI��CSƬѡ����
    ** BL /LED     ���Ʊ���LED�� �ߵ�ƽ���������Ѵ�1K���裬����10K������
    **  
    ** ����Ӳ��    ħŮ������_STM32F103RCT6 + 1.8�����ģ��(ST7735)
************************************************************************************************************************************/
//#include "manage_f103.h"	 

#include "delay.h"


/*****************************************************************************
 ** ��ֲʱ��ֻ���޸���������š�SPI�˿ڡ���Ļ����
*****************************************************************************/
// ģ������
// 3-SCL
#define LCD_SCK_GPIO      GPIOA             // ʱ�ӣ�SCL/SCK
#define LCD_SCK_PIN       GPIO_Pin_5
// 4-SDA
#define LCD_MOSI_GPIO     GPIOA            // �����SDA/DIN
#define LCD_MOSI_PIN      GPIO_Pin_7
// 5-RES
#define LCD_RES_GPIO      GPIOB             // ��λ, RST
#define LCD_RES_PIN       GPIO_Pin_6
// 6-DC
#define LCD_DC_GPIO       GPIOB             // ����, RS/DC  
#define LCD_DC_PIN        GPIO_Pin_7
// 7-CS
#define LCD_CS_GPIO       GPIOB             // Ƭѡ, CS/CE
#define LCD_CS_PIN        GPIO_Pin_8
// 8-BL
#define LCD_BL_GPIO       GPIOB             // ���⣬BL/LED
#define LCD_BL_PIN        GPIO_Pin_9
// SPI
#define LCD_SPI_PORT      SPI1
#define LCD_SPI_SCK       RCC_APB2Periph_SPI1
// ��Ļ����
#define LCD_WIDTH         129               // ������Ļ�����ش�С��
#define LCD_HIGH          160               // ע�⣺ʵ�����غ���Ҫ�ȱ�ʾֵ��1~2���أ�
#define LCD_DIR           1                 // ������ʾ���򣬲�����1��2��3��4
//  ���� ��ֲ�޸� ************************************************************




  
/*****************************************************************************
 ** ��ɫ���壬 ��ֲʱ�����޸�
****************************************************************************/
#define WHITE         	 0xFFFF
#define BLACK         	 0x0000	  
#define BLUE         	 0x001F  
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //��ɫ
#define BRRED 			 0XFC07 //�غ�ɫ
#define GRAY  			 0X8430 //��ɫ
#define DARKBLUE      	 0X01CF	//����ɫ
#define LIGHTBLUE      	 0X7D7C	//ǳ��ɫ  
#define GRAYBLUE       	 0X5458 //����ɫ 
#define LIGHTGREEN     	 0X841F //ǳ��ɫ
#define LIGHTGRAY        0XEF5B //ǳ��ɫ(PANNEL)
#define LGRAY 			 0XC618 //ǳ��ɫ(PANNEL),���屳��ɫ
#define LGRAYBLUE        0XA651 //ǳ����ɫ(�м����ɫ)
#define LBBLUE           0X2B12 //ǳ����ɫ(ѡ����Ŀ�ķ�ɫ)
	    			


/*****************************************************************************
 ** ����  ȫ�ֺ����� ��ֲʱ�����޸�
****************************************************************************/
void LCD_Init(void);                                                        // LCD����ST7735��ʼ������Ļ�����ڱ��ļ������޸�(��ֲ������)����Ҫ�����سߴ硢��ʾ����
void LCD_Display(u8 sw);                                                    // ��ʾ�����أ� 1������Ļ�� 0��Ϩ��
void LCD_Fill(u16 sx, u16 sy, u16 ex, u16 ey, u16 color);		            // ��䵥ɫ
void LCD_Line(u16 sX, u16 sY, u16 eX, u16 eY, u16 color);                   // ����
void LCD_Circle(u16 x0,u16 y0,u8 r, u16 color);				                // ��Բ
void LCD_Image(u16 x, u16 y, u16 width, u16 height, const u8 *image);       // ��ʾͼƬ
// �����ַ���������ʾ���ַ�������font.h�ļ��У�����������W25Q128�ļ��У��ʺ����⺺����������壺12��16��24��32��
void LCD_String(u16 x, u16 y, char* pFont, u8 size, u32 fColor, u32 bColor);  // ��ʾ��Ӣ�ַ��������Ͻ�����x, ���Ͻ�����y, �ֿ⻺���ַ���ֺŴ�С����ɫ����ɫ
// �ض�������ʾ:�ֿ�������font�ļ���.ֻ�ܱ��������ֿ⣬�ʺϹ̶����

void LCD_Chinese16x16ForFile(u32 x, u32 y, u8 *s, u16 fColor, u16 bColor);  // ��ʾ1��16�ź��֣����Ͻ�����x, ���Ͻ�����y, �ֿ������ڻ����е�λ�ã���ɫ����ɫ

void LCD_Chinese16ForFile(u32 x, u32 y, u8 index, u16 fColor, u16 bColor);  // ��ʾ1��16�ź��֣����Ͻ�����x, ���Ͻ�����y, �ֿ������ڻ����е�λ�ã���ɫ����ɫ
void LCD_Chinese32ForFile(u32 x, u32 y, u8 index, u16 fColor, u16 bColor);  // ��ʾ1��32�ź��֣����Ͻ�����x, ���Ͻ�����y, �ֿ������ڻ����е�λ�ã���ɫ����ɫ
//void LCD_GUI(void);

/*Ҫ�õ��Ķ̾�*/
void LCD_CHINSES_BYSJ(u8 x,u8 y); //��ҵ���
void LCD_CHINSES_GetPassWord(u8 x, u8 y);//ѡ��


void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey);//��ʾ���ִ�
#endif  
	 
	 



