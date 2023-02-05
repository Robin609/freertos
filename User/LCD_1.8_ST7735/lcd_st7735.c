/***********************************************************************************************************************************
    *	@file  	  lcd_st7735.c
	*   @date     2020-08-17		
	*	@author   ħŮ������   ����
    *   @brief    �������     QȺ�ļ��У�262901124  ��������
    *   @taobao   �Ա�Ӳ��     https://demoboard.taobao.com/
    ********************************************************************************************************************************
    ** 1.8���ɫ��ʾ��ģ������˵��(ST7735)
    ** GND         ��Դ��
    ** VCC         3.3V����Ҫ��5V, ��������
    ** SCL/SCK     SPI��SCK����  
    ** SDA/DIN     SPI��MOSI���� 
    ** RES/RST     ��λ���ţ���ú͵�Ƭ���ĸ�λ�������
    ** DC /RS      �������ѡ������
    ** CS /CE      SPI��CSƬѡ����
    ** BL /LED     ���Ʊ���LED�� �ߵ�ƽ���������Ѵ�1K���裬����10K������
    **  
    ** ����Ӳ��    ħŮ������_STM32F103RCT6 + 1.8�����ģ��(ST7735)
************************************************************************************************************************************/
#include "lcd_st7735.h"
#include "font.h"
#include "stm32f10x_spi.h"
//#include "w25qxx.h"



/*****************************************************************************
 ** ���ر���
 *****************************************************************************/
struct _LCD              // �ṹ�壺���ڱ�����Ϣ��״̬
{     
    u8  InitOK;
	u16 width;			 // LCD ���
	u16 height;			 // LCD �߶�
    
    u32 bColor;          // ����ɫ
}xLCD;



/*****************************************************************************
 ** ���غ���
 ****************************************************************************/
void gpioInit(void);                                         // ���ų�ʼ��
void spiInit(void);                                          // SPIͨ��Э���ʼ��
void sendOrder(u16 order);                                   // ��Һ�������ͣ��Ĵ�����ֵַ(ָ��), 8λ
void sendByte (u8  Data);                                    // ��Һ�������ͣ����ݣ�8λ
void sendShort(u16 Data);                                    // ��Һ�������ͣ����ݣ�16λ
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd);  // ������ʾ����




/*****************************************************************************
 ** ��ʾ�����ƺ궨��, ��ҪΪ�˼��ļ��ڲ���
 *****************************************************************************/
// LCD_CS  
#define	LCD_CS_LOW  	LCD_CS_GPIO->BSRR=LCD_CS_PIN <<16
#define LCD_CS_HIGH     LCD_CS_GPIO->BSRR=LCD_CS_PIN  
// 
#define	LCD_BL_LOW  	LCD_BL_GPIO->BSRR=LCD_BL_PIN  <<16 
#define	LCD_BL_HIGH  	LCD_BL_GPIO->BSRR=LCD_BL_PIN 

#define	LCD_RS_LOW  	LCD_DC_GPIO->BSRR=LCD_DC_PIN <<16
#define	LCD_RS_HIGH  	LCD_DC_GPIO->BSRR=LCD_DC_PIN 
//LCD_RST
#define	LCD_RES_LOW  	LCD_RES_GPIO->BSRR=LCD_RES_PIN <<16
#define	LCD_RES_HIGH  	LCD_RES_GPIO->BSRR=LCD_RES_PIN 
 	




// ���ų�ʼ��
void gpioInit(void)
{
#if 0
    // ʹ��ħŮ�������Ŷӱ�д�ĳ�ʼ����������ֱ������·���׼��
    GPIOSet (LCD_SCK_GPIO , LCD_SCK_PIN , GPIO_Mode_AF ,  GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
    GPIOSet (LCD_MOSI_GPIO, LCD_MOSI_PIN, GPIO_Mode_AF ,  GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
    GPIOSet (LCD_RES_GPIO , LCD_RES_PIN , GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
    GPIOSet (LCD_DC_GPIO ,  LCD_DC_PIN ,  GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
	GPIOSet (LCD_CS_GPIO ,  LCD_CS_PIN ,  GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );
	GPIOSet (LCD_BL_GPIO ,  LCD_BL_PIN ,  GPIO_Mode_OUT , GPIO_OType_PP , GPIO_Speed_50MHz, GPIO_PuPd_UP );	
#else
	// ʹ�ñ�׼��
	GPIO_InitTypeDef GPIO_InitStructure;
	RCC_APB2PeriphClockCmd(	RCC_APB2Periph_GPIOA|RCC_APB2Periph_GPIOB|RCC_APB2Periph_GPIOC, ENABLE );// PORTʱ��ʹ�� 
    // SCK
	GPIO_InitStructure.GPIO_Pin   = LCD_SCK_PIN  ;	  
	GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP;           // ����������� 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(LCD_SCK_GPIO, &GPIO_InitStructure);
    // MOSI
	GPIO_InitStructure.GPIO_Pin   = LCD_MOSI_PIN ;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_AF_PP; 
    GPIO_Init(LCD_MOSI_GPIO , &GPIO_InitStructure);
    // RES
	GPIO_InitStructure.GPIO_Pin   = LCD_RES_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_RES_GPIO , &GPIO_InitStructure);
    // DC
	GPIO_InitStructure.GPIO_Pin   = LCD_DC_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_DC_GPIO , &GPIO_InitStructure);
    // CS
	GPIO_InitStructure.GPIO_Pin   = LCD_CS_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_CS_GPIO , &GPIO_InitStructure);
	// BL
	GPIO_InitStructure.GPIO_Pin   = LCD_BL_PIN;  
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz; 
    GPIO_InitStructure.GPIO_Mode  = GPIO_Mode_Out_PP; 
    GPIO_Init(LCD_BL_GPIO , &GPIO_InitStructure);
#endif
	LCD_CS_HIGH ;   // ����������ֹ��ƽ����
	LCD_BL_HIGH ;   // ����Ļ��ʾ	
}

// SPIͨ��Э���ʼ��
// ��Ҫ����ΪSPI���߿��ܹ��ض��Ҫ��ͬͨ�Ų������豸�������ÿ��ͨ��ǰ������һ�����ú��� 
void spiInit(void)
{  
	SPI_InitTypeDef  SPI_InitStructure;
	RCC_APB2PeriphClockCmd(	LCD_SPI_SCK,  ENABLE ); // SPI1ʱ��ʹ�� 	

	SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;  //����SPI�������˫�������ģʽ:SPI����Ϊ˫��˫��ȫ˫��  SPI_Direction_1Line_Tx
	SPI_InitStructure.SPI_Mode = SPI_Mode_Master;		    // ����SPI����ģʽ:����Ϊ��SPI
	SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;		// ����SPI�����ݴ�С:SPI���ͽ���8λ֡�ṹ
	SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;		        // ѡ���˴���ʱ�ӵ���̬:ʱ�����ո�
	SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;	        // ���ݲ����ڵڶ���ʱ����
	SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;		        // NSS�ź���Ӳ����NSS�ܽţ����������ʹ��SSIλ������:�ڲ�NSS�ź���SSIλ����
	SPI_InitStructure.SPI_BaudRatePrescaler =SPI_BaudRatePrescaler_2;		//���岨����Ԥ��Ƶ��ֵ:������Ԥ��ƵֵΪ16
	SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;	    // ָ�����ݴ����MSBλ����LSBλ��ʼ:���ݴ����MSBλ��ʼ
	SPI_InitStructure.SPI_CRCPolynomial = 7;	            // CRCֵ����Ķ���ʽ
	SPI_Init(SPI1, &SPI_InitStructure);                     // ����SPI_InitStruct��ָ���Ĳ�����ʼ������SPIx�Ĵ���
 
	SPI_Cmd(LCD_SPI_PORT, ENABLE);                                  // ʹ��SPI����	
	//SPI2_ReadWriteByte(0xff);//��������	
}

// SPI���ʹ���1�ֽ�
u8 spiSendByte(u8 data)
{
    static u16 retry;
    
	retry=0;	
    while((LCD_SPI_PORT -> SR & 2) == 0) {  // �ȴ�������Ϊ��	
		retry++;
		if(retry>500) return 0;
	}
    LCD_SPI_PORT ->DR = data;
	
	retry =0;    
    while((LCD_SPI_PORT -> SR & 1) == 0){   // �ȴ�����������	
		retry++;
		if(retry>250) return 0;
	}
    return LCD_SPI_PORT -> DR ;       
}		   



/*****************************************************************************
*��  ����LCD_Init
*��  �ܣ���ʼ��lcd
*��  ����Ϊ�˼���⡢������ֲ������(��Ļ���ء���ʾ����)��h�ļ�������޸�
*����ֵ���� 
*��  ע��
*****************************************************************************/
void LCD_Init(void)  
{ 	 
    xLCD.InitOK = 0; 
    xLCD.bColor  = BLACK;
    
	// ��ʾ��������ƥ��
    if( (LCD_DIR ==1) || (LCD_DIR ==3))
    {
        xLCD.width  = LCD_WIDTH ;       // ��������أ�������ֵ����оƬ���Զ����У�ע�⣺�����Ļ�ұ��л������ͼӴ����ֵ
        xLCD.height = LCD_HIGH ;        // ���߶����أ� ע�⣺�����Ļ�����л������ͼӴ����ֵ
    }
    else
    {
        xLCD.width  = LCD_HIGH;
        xLCD.height = LCD_WIDTH;
    }      
	
	// ��ʼ��GPIO
	gpioInit();
	// ��ʼ��SPI���衢Э��
    spiInit();
	
	// ��Ļ��λ	
	LCD_RES_LOW ;	       // LCD_RST=0	  //SPI�ӿڸ�λ
	delay_ms(20);          // delay 20 ms 
    LCD_RES_HIGH  ;        // LCD_RST=1		
	delay_ms(20);

	sendOrder(0x11);       // �˳�˯��ģʽ
	delay_ms(120);         // Delay 120ms
	//------------------------------------ST7735S Frame Rate-----------------------------------------//
	sendOrder(0xB1);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
    
	sendOrder(0xB2);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
    
	sendOrder(0xB3);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
	sendByte (0x05);
	sendByte (0x3C);
	sendByte (0x3C);
	//------------------------------------End ST7735S Frame Rate-----------------------------------------//
	sendOrder(0xB4); // Dot inversion
	sendByte(0x03);
    
	sendOrder(0xC0); // ST7735R Power Sequence
	sendByte(0x28);
	sendByte(0x08);
	sendByte(0x04);
    
	sendOrder(0xC1);
	sendByte(0XC0);
    
	sendOrder(0xC2);
	sendByte(0x0D);
	sendByte(0x00);
	sendOrder(0xC3);
    
	sendByte(0x8D);
	sendByte(0x2A);
    
	sendOrder(0xC4);
	sendByte(0x8D);
	sendByte(0xEE);
	//---------------------------------End ST7735S Power Sequence-------------------------------------//
	sendOrder(0xC5); //VCOM
	sendByte(0x1A);
	sendOrder(0x36); //MX, MY, RGB mode
	if(LCD_DIR == 1)  sendByte(0xC0);  // ��Ҫ����ʾ������ƣ�C0/00/A0/60,  C8/08/A8/68
	if(LCD_DIR == 2)  sendByte(0x00);      
    if(LCD_DIR == 3)  sendByte(0xA0);
    if(LCD_DIR == 4)  sendByte(0x60);
	//------------------------------------ST7735S Gamma Sequence-----------------------------------------//
	sendOrder(0xE0);
	sendByte(0x04);
	sendByte(0x22);
	sendByte(0x07);
	sendByte(0x0A);
	sendByte(0x2E);
	sendByte(0x30);
	sendByte(0x25);
	sendByte(0x2A);
	sendByte(0x28);
	sendByte(0x26);
	sendByte(0x2E);
	sendByte(0x3A);
	sendByte(0x00);
	sendByte(0x01);
	sendByte(0x03);
	sendByte(0x13);
    
	sendOrder(0xE1);
	sendByte(0x04);
	sendByte(0x16);
	sendByte(0x06);
	sendByte(0x0D);
	sendByte(0x2D);
	sendByte(0x26);
	sendByte(0x23);
	sendByte(0x27);
	sendByte(0x27);
	sendByte(0x25);
	sendByte(0x2D);
	sendByte(0x3B);
	sendByte(0x00);
	sendByte(0x01);
	sendByte(0x04);
	sendByte(0x13);
    
    // ���ڸ������ӵģ�������
    sendOrder(0x2a);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x7f);
    // ���ڸ������ӵģ�������
	sendOrder(0x2b);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x00);
	sendByte(0x9f);
	//------------------------------------End ST7735S Gamma Sequence-----------------------------------------//
	sendOrder(0x3A); // 65k mode
	sendByte(0x05);
	
    sendOrder(0x29); // Display on

	LCD_Fill(1, 1, xLCD.width , xLCD.height , BLACK );
    xLCD.InitOK = 1; 	
}  
// ��LCD���ͼĴ�����ַ(ָ��)
void sendOrder(u16 order)
{ 
	LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
    LCD_RS_LOW;            // RS��: ����ֵ�� RS��: �Ĵ�����ֵַ
	spiSendByte(order);    // ���� �Ĵ�����ֵַ
	LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 
}
// ��LCD������ֵ
void sendByte(u8 data) 
{
 	LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
	LCD_RS_HIGH;	       // RS��: ����ֵ�� RS��: �Ĵ�����ֵַ
	spiSendByte(data);     // ����1�ֽ�
	LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 			 
}	
// ��LCD����2���ֽ�
void sendShort(u16 data)
{
 	LCD_CS_LOW;            // SPI�豸Ƭѡ���ͣ���ʼͨ��
	LCD_RS_HIGH;	       // RS��: ���ݣ� RS��: ָ��
	spiSendByte(data>>8);  // ����1�ֽ�
	spiSendByte(data);     // ����1�ֽ�
	LCD_CS_HIGH;           // SPI�豸Ƭѡ���ߣ�����ͨ�� 	
}
				   
//д�Ĵ���: �Ĵ�����ַ������
void sendRegisterValue(u8 reg, u16 val)
{
	sendOrder(reg);
	sendShort(val);
}	 
	
/*****************************************************************
 * ��  ����LCD_Display
 * ��  �ܣ���ʾ������
 * ��  ����0:Ϩ����0����
 * ����ֵ����
 *
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
******************************************************************/
void LCD_Display(u8 sw)
{
	if(sw==0)
	    LCD_BL_LOW ;
	else
		LCD_BL_HIGH ;	
}
	
/*****************************************************************
 * ��  ����setCursor
 * ��  �ܣ�������ʾ�����ڴ�����д�������Զ�����
 * ��  ����xStart���������, yStart���������
 *         xEnd�����������㣬yEnd������������
 * ����ֵ����
 *
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
******************************************************************/
void setCursor(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd)
{		
	sendOrder(0x2A);
	sendByte (0x00);   // ��ʼλ��x��λ����Ϊ1.8������128*160, ������255, ֱ��д0ʡ��
	sendByte (xStart); // ��ʼλ��x��λ��ֵ����ʱ�Զ������˸�8λ��Ҳʡ��������
	sendByte (0x00);   // ��ʼλ��y��λ
	sendByte (xEnd);   // ��ʼλ��xλλ

	sendOrder(0x2B);
	sendByte (0x00);
	sendByte (yStart);
	sendByte (0x00);
	sendByte (yEnd);
	
	sendOrder(0x2c);  // ����д��������
}

/*****************************************************************
 * ��  ����drawPoint
 * ��  �ܣ���һ����
 * ��  ����x���꣬y����, 16λ��ɫֵ
 * ����ֵ����
 * 
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
******************************************************************/
void drawPoint(u16 x, u16 y, u16 color)
{
    setCursor(x, y, x, y);      //���ù��λ�� 
    sendShort(color);    
}

/******************************************************************
 * �������� LCD_Fill
 * ��  �ܣ� ��ָ����������䵥����ɫ
 *          ��Ļ�����1��ʼ����Ļ���Һ��·�ʵ�����ж�������
 * ��  ���� u16 xStart, yStart  ���Ͻ���ʼ����
 *          u16 xEnd,   yEnd    ���½���ʼ����
 *          u32 Color           ������ɫ
 *
 * ��  ע�� ħŮ�������Ŷӱ�д   �Ա� https://demoboard.taobao.com
 * ��  �� ���ϸ�����QȺ�ļ��У�262901124  ����޸�_2020��09��01��
 *****************************************************************/
void LCD_Fill(u16 xStart, u16 yStart, u16 xEnd, u16 yEnd, u16 color)
{     
	spiInit();                                      // ��ֹSPI�����������豸�޸���
	u32 pixel = (xEnd-xStart+1) * (yEnd-yStart+1);	// ������������
								   
	setCursor(xStart, yStart, xEnd, yEnd);      	// �趨��䷶Χ
    while(pixel-->0)                                // ������ɫֵ
       sendShort(color);                            
}  

/******************************************************************
 * �������� LCD_Line
 * ��  �ܣ� ����
 * ��  ���� u16 x1,y1  �������
 *          u16 x2,y2  �յ����� 
 *          u32 Color  ��ɫ
 *****************************************************************/
void LCD_Line(u16 sX, u16 sY, u16 eX, u16 eY, u16 color) 
{
	spiInit();                                      // ��ֹSPI�����������豸�޸���
	
	u16 t; 
	int xerr=0, yerr=0, delta_x, delta_y, distance; 
	int incx,incy,uRow,uCol; 
	delta_x = eX - sX; //������������ 
	delta_y = eY - sY; 
	uRow = sX; 
	uCol = sY; 
	if(delta_x>0)incx=1; //���õ������� 
	else if(delta_x==0)incx=0;//��ֱ�� 
	else {incx=-1;delta_x=-delta_x;} 
	if(delta_y>0)incy=1; 
	else if(delta_y==0)incy=0;//ˮƽ�� 
	else{incy=-1;delta_y=-delta_y;} 
	if( delta_x>delta_y)distance=delta_x; //ѡȡ�������������� 
	else distance=delta_y; 
	for(t=0;t<=distance+1;t++ )//������� 
	{  
		drawPoint(uRow, uCol, color);//���� 
		xerr+=delta_x ; 
		yerr+=delta_y ; 
		if(xerr>distance) 
		{ 
			xerr-=distance; 
			uRow+=incx; 
		} 
		if(yerr>distance) 
		{ 
			yerr-=distance; 
			uCol+=incy; 
		} 
	}  
}    

//��ָ��λ�û�һ��ָ����С��Բ
//(x,y):���ĵ�
//r    :�뾶
// ԭ�����ṩ�Ļ�Բ�������о��е�ʧ�棬����������������
void LCD_Circle(u16 x0,u16 y0,u8 r, u16 color)
{
	spiInit();                                   // ��ֹSPI�����������豸�޸���
	int a,b;
	int di;
	a=0;b=r;	  
	di=3-(r<<1);                                 //�ж��¸���λ�õı�־
	while(a<=b)
	{
		drawPoint(x0+a,y0-b, color);             //5
 		drawPoint(x0+b,y0-a, color);             //0           
		drawPoint(x0+b,y0+a, color);             //4               
		drawPoint(x0+a,y0+b, color);             //6 
		drawPoint(x0-a,y0+b, color);             //1       
 		drawPoint(x0-b,y0+a, color);             
		drawPoint(x0-a,y0-b, color);             //2             
  		drawPoint(x0-b,y0-a, color);             //7     	         
		a++;
		//ʹ��Bresenham�㷨��Բ     
		if(di<0)di +=4*a+6;	  
		else
		{
			di+=10+4*(a-b);   
			b--;
		} 						    
	}
} 

/******************************************************************
 * �������� LCD_Image
 * ��  �ܣ� ��ָ�����������ָ��ͼƬ����
 *          ͼƬ������font.h�ļ���.ֻ�ʺ�����ͼƬ����
 *          Image2Lcdת����ˮƽɨ�裬16λ���ɫ
 * ��  ���� u16 x,y     ���Ͻ���ʼ����
 *          u16 width   ͼƬ���
 *          u16 height  ͼƬ�߶�
 *          u8* image   ���ݻ����ַ
 * ��  ע�� ħŮ�������Ŷ�  ���ϴ��QȺ��262901124        ����޸�_2020��09��2��
 *****************************************************************/
void LCD_Image(u16 x, u16 y, u16 width, u16 height, const u8 *image) 
{  
	spiInit();                                 // ��ֹSPI�����������豸�޸���
	
	u16 colorData=0;
	u32 cnt=0;
	
 	for(u16 i=0; i<height; i++)                // һ��һ�е���ʾ
	{
 		setCursor(x, y+i, x+width, y+height);  // �������ù��λ�� 
		for(u16 j=0; j<width; j++)             // һ���У������£�������ش���
		{		    
		    colorData=(image[cnt*2+1]<<8) | image[cnt*2]; 
		    sendShort(colorData);              // д��16λ��ɫ���� 
			cnt++;	
		}
	}	  
}  
/******************************************************************************
      ����˵������ʾ���ִ�
      ������ݣ�x,y��ʾ����
                *s Ҫ��ʾ�ĺ��ִ�
                fc �ֵ���ɫ
                bc �ֵı���ɫ
                sizey �ֺ� ��ѡ 16 24 32
                mode:  0�ǵ���ģʽ  1����ģʽ
      ����ֵ��  ��
******************************************************************************/
void LCD_ShowChinese(u16 x,u16 y,u8 *s,u16 fc,u16 bc,u8 sizey)
{
	printf ("%c",s);
//	putc(s);
	while(*s!=0)
	{
		if(sizey==16) LCD_Chinese16x16ForFile(x,y,s,fc,bc);
//		else if(sizey==24) LCD_ShowChinese24x24(x,y,s,fc,bc,sizey,mode);
//		else if(sizey==32) LCD_ShowChinese32x32(x,y,s,fc,bc,sizey,mode);
		else return;
		s+=2;
		x+=sizey;
	}
}
/******************************************************************
 * �������� LCD_Chinese16ForFile
 * ��  �ܣ� ��ʾ1��16�ź���,�ֿ�������font�ļ��У�ֻ�ʺ��������̶ֹ����
 *          PCtoLCD2002ȡģ������+����ʽ+˳��+C51��ʽ
 * ��  ���� u16 x,y     ���Ͻ���ʼ����
 *          u8  index   �ֿ������ڻ����е�λ��
 *          u32 fColor  ��ɫ
 *          u32 bColor  ��ɫ
 *****************************************************************/
void LCD_Chinese16x16ForFile(u32 x, u32 y, u8 *s, u16 fColor, u16 bColor)	
{  
	spiInit();                                      // ��ֹSPI�����������豸�޸���
	u8 i,j;
	u16 k;
	u16 HZnum;//������Ŀ
	u16 TypefaceNum;//һ���ַ���ռ�ֽڴ�С
	u16 x0=x;
	u8 sizey =16;
	TypefaceNum=sizey/8*sizey;//���㷨ֻ�������ֿ�����ָߣ����ָ���8�ı������֣�
	                          //Ҳ�����û�ʹ��������С����,������ʾ���׳����⣡
	HZnum=sizeof(tfont16)/sizeof(typFNT_GB16);	//ͳ�ƺ�����Ŀ
	for(k=0;k<HZnum;k++) 
	{
		if ((tfont16[k].Index[0]==*(s))&&(tfont16[k].Index[1]==*(s+1)))
		{ 	
			//LCD_Address_Set(x,y,x+sizey-1,y+sizey-1);
			setCursor(x,y+j ,x+16, y+j+16);
			for(i=0;i<TypefaceNum;i++)
			{
				for(j=0;j<8;j++)
				{	
				  if((tfont16[k].Msk[i]&(1<<i))!=0)			
					  sendShort(fColor);				
				  else			
					  sendShort(bColor);
				
				}
			}
		}				  	
	//	continue;  //���ҵ���Ӧ�����ֿ������˳�����ֹ��������ظ�ȡģ����Ӱ��
	}

	
//	const u8 *temp=hanzi16 + index*32;              // ��ȡ���������������еĵ���ʼλ�� 

//	for(u8 j=0;j<16;j++)
//	{
//		setCursor(x,y+j ,x+16, y+j+16);
//		for(u8 k=0;k<2;k++)
//		{
//			for(u8 i=0;i<8;i++)
//			{ 		     
//			 	if((*temp&(1<<i))!=0)			
//					sendShort(fColor);				
//				else			
//					sendShort(bColor);				 
//			}
//			temp++;
//		}
//	 }
}	
/******************************************************************
 * �������� LCD_Chinese16ForFile
 * ��  �ܣ� ��ʾ1��16�ź���,�ֿ�������font�ļ��У�ֻ�ʺ��������̶ֹ����
 *          PCtoLCD2002ȡģ������+����ʽ+˳��+C51��ʽ
 * ��  ���� u16 x,y     ���Ͻ���ʼ����
 *          u8  index   �ֿ������ڻ����е�λ��
 *          u32 fColor  ��ɫ
 *          u32 bColor  ��ɫ
 *****************************************************************/
void LCD_Chinese16ForFile(u32 x, u32 y, u8 index, u16 fColor, u16 bColor)	
{  
	spiInit();                                      // ��ֹSPI�����������豸�޸���
	const u8 *temp = hanzi16 + index*32;              // ��ȡ���������������еĵ���ʼλ�� 

	for(u8 j=0;j<16;j++)
	{
		setCursor(x,y+j ,x+16, y+j+16);
		for(u8 k=0;k<2;k++)
		{
			for(u8 i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)			
					sendShort(fColor);				
				else			
					sendShort(bColor);				 
			}
			temp++;
		}
	 }
 }
/******************************************************************
 * �������� LCD_Chinese32ForFile
 * ��  �ܣ� ��ʾ1��32�ź���,�ֿ�������font�ļ��У�ֻ�ʺ��������̶ֹ����
 *          PCtoLCD2002ȡģ������+����ʽ+˳��+C51��ʽ
 * ��  ���� u16 x,y     ���Ͻ���ʼ����
 *          u8  index   �ֿ������ڻ����е�λ��
 *          u32 fColor  ������ɫ
 *          u32 bColor  ������ɫ
 *****************************************************************/
void LCD_Chinese32ForFile(u32 x, u32 y, u8 index, u16 fColor, u16 bColor)	
{ 
	spiInit();                                  // ��ֹSPI�����������豸�޸���
	
	const u8 *temp = hanzi32 + index*128;       // ��ȡ���������������еĵ���ʼλ�� 

	for(u8 j=0; j<32; j++)                      // һ��һ�е�����д
	{
		setCursor(x,y+j,x+32, y+j+32);          // ֻҪ��λ���׸����꣬���淢�͵����ݣ��ͻ�������У�ֱ���յ��µ�����
		for(u8 k=0;k<4;k++)
		{
			for(u8 i=0;i<8;i++)
			{ 		     
			 	if((*temp&(1<<i))!=0)				
					sendShort(fColor);				
				else
					sendShort(bColor);				 
			}
			temp++;
		}
	 }
}	

/******************************************************************
 * �������� drawAscii
 * ��  �ܣ� ��ָ��λ����ʾһ���ַ�	
 * ��  ���� u16 x,y     ��ʼ����
 *          u8  num     Ҫ��ʾ���ַ�:" "--->"~"
 *          u8  size    �����С 12/16/24/32
 *          u32 fColor  ������ɫ
 *          u32 bColor  ������ɫ
 * ��  ע�� �ο�ԭ�Ӹ��Ұ�����Ĵ�����޸�  ���ϴ��QȺ��262901124 
 *****************************************************************/
void drawAscii(u16 x,u16 y,u8 num,u8 size,u32 fColor, u32 bColor)
{  			
	spiInit();                                        // ��ֹSPI�����������豸�޸���
	
    if(xLCD.InitOK ==0) return;
    
    u8 temp;
	u16 y0=y;
    
	u8 csize=(size/8+((size%8)?1:0))*(size/2);		   // �õ�����һ���ַ���Ӧ������ռ���ֽ���	
 	num=num-' ';                                       // �õ�ƫ�ƺ��ֵ��ASCII�ֿ��Ǵӿո�ʼȡģ������-' '���Ƕ�Ӧ�ַ����ֿ⣩
	for(u8 t=0;t<csize;t++)
	{   
		if(size==12)         temp=asc2_1206[num][t];   // ����1206����
		else if(size==16)    temp=asc2_1608[num][t];   // ����1608����
		else if(size==24)    temp=asc2_2412[num][t];   // ����2412����
		else if(size==32)    temp=asc2_3216[num][t];   // ����3216����
		else return;								   // û�е��ֿ�
		
		for(u8 t1=0; t1<8; t1++)
		{			    
            if(temp&0x80)   drawPoint (x, y, fColor);  // ���� ���� 
            else            drawPoint (x, y, bColor);  // ���� ����
            temp<<=1;
			y++;
			if(y>=xLCD.height)    return;		       // ������Ļ�߶�(��)
			if((y-y0)==size)
			{
				y=y0;
				x++;
				if(x>=xLCD.width) return;              // ������Ļ���(��)
				break;
			}
		}  	 
	}  	    	 	  
} 

// code �ַ�ָ�뿪ʼ
// ���ֿ��в��ҳ���ģ
// code �ַ����Ŀ�ʼ��ַ,GBK��
// mat  ���ݴ�ŵ�ַ (size/8+((size%8)?1:0))*(size) bytes��С	
// size:�����С
void GetGBK(u8* code, u8* mat, u8 size)
{		    
	u8 qh,ql;						  
	u32 foffset; 
	u8 csize=(size/8+((size%8)?1:0))*(size);    // ���㺺�ֵ����С����λ�ֽ���	 
	
    qh=*code;
	ql=*(++code);    
	
	if(qh<0x81||ql<0x40||ql==0xff||qh==0xff)    // �ǳ��ú��֣����������ʾ����λ��
	{   	          
	    for(u8 i=0; i<csize; i++) *mat++=0x00;  // �������
	    return;                                 // ��������
	}     

	if(ql<0x7f)ql-=0x40;                        // ����Ҫ��ȡ�ĺ������ֿ��е�ƫ��λ��
	else ql-=0x41;
	qh-=0x81;   
	foffset=((unsigned long)190*qh+ql)*csize;	// �õ��������ֿ��е�ƫ��λ��  		

	switch(size)
	{                                                                                 // ������Ĳ�ͬ���ڲ�ͬ�ֿ��ȡ�������
		case 12:
			//W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR,            mat, csize);    // 12������      ħŮ�������ϵ�FLASH�����ֿ⣬      
			break;  
		case 16:
            //W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR+0x0008c460, mat, csize);    // 16������      ħŮ�������ϵ�FLASH�����ֿ⣬
			break;
		case 24:
            //W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR+0x001474E0, mat, csize);    // 24������      ħŮ�������ϵ�FLASH�����ֿ⣬
			break;
		case 32:
            //W25qxx_ReadBuffer( foffset + GBK_STORAGE_ADDR+0x002EC200, mat, csize);    // 32������      ħŮ�������ϵ�FLASH�����ֿ⣬
			break;			
	}     												    
}  

// ��ʾ����
void drawGBK( u16 x, u16 y, u8* font, u8 size, u32 fColor, u32 bColor)
{	
    u8 temp;
	u16 y0=y;
	u8 GBK[128];   
	u8 csize=(size/8+((size%8)?1:0))*(size);	// �õ�����һ���ַ���Ӧ������ռ���ֽ���	 
	GetGBK(font, GBK, size);	                // �õ���Ӧ��С�ĵ�������
	
	spiInit();                                  // ��ֹSPI�����������豸�޸���	
	for(u8 t=0; t<csize; t++)
	{   												   
		temp = GBK[t];			                // �õ�GBK��������                          
		for(u8 t1=0; t1<8; t1++)
		{
			if(temp&0x80)   drawPoint (x, y, fColor);	
            else            drawPoint (x, y, bColor);            
			temp<<=1;
			y++;
			if((y-y0)==size)
			{
				y=y0;
				x++;
				break;
			}
		}  	 
	}      
}



/******************************************************************************
 * ��  ���� LCD_String
 * ��  �ܣ� ����Ļ����ʾ�ַ�����֧��Ӣ�ġ�����
            ��Ҫ��ʾ��ע���ֿ�ĸ�ʽ��λ�ã� �������ֿ���W25Q128��
 *         
 * ��  ���� 
 * ����ֵ����
 * ��  ע�� ħŮ�������Ŷ�  ���ϴ��QȺ��262901124        ����޸�_2020��05��1����
 ******************************************************************************/  
void LCD_String(u16 x, u16 y, char* pFont, u8 size, u32 fColor, u32 bColor)
{
    if(xLCD .InitOK ==0 ) return;
    
    u16 xStart = x;    

    if( size!=12 && size!=16 && size!=24 && size!=32 )       // �����С����
        size=24;    
    
    while(*pFont != 0)                // ������ȡ�ַ������ݣ�ֱ��'\0'ʱֹͣ
    {    
        if(x>(xLCD.width-size))       // ��λ���жϣ����������ĩ���Ͱѹ�껻��
        {
            x=xStart;
            y=y+size;
        }
        if(y>(xLCD.height - size))    // ��λ���жϣ����������ĩ���ͷ��أ��������
            return;        
        
        if(*pFont < 127)              // ASCII�ַ�
        {
            drawAscii (x, y, *pFont, size, fColor, bColor);            
            pFont++;
            x+=size/2;            
        }
        else                          // ������ʾ
        {     
            // ��Ҫ: ����õĲ���ħŮ��������ֿ�, ��Ҫ�޸Ļ�ע��������һ��, �����Ͳ�Ӱ��ASCIIӢ���ַ������            
            drawGBK(x, y, (u8*)pFont, size, fColor, bColor);     
            pFont = pFont+2;          // ��һ��Ҫ��ʾ���������ڴ��е�λ��              
            x=x+size;                 // ��һ��Ҫ��ʾ����������Ļ�ϵ�Xλ��    
        }
    }      
}
/******************************************************************************
 * ��  ���� LCD_CHINSES_BYSJ
 * ��  �ܣ� ����Ļ����ʾ�ַ�����֧��Ӣ�ġ�����
            
 *         
 * ��  ���� 
 * ����ֵ����
 * ��  ע�� 
 ******************************************************************************/ 
void LCD_CHINSES_BYSJ(u8 x,u8 y)
{
	LCD_Chinese16ForFile(x, y,0, BRRED, BLACK);
	LCD_Chinese16ForFile(x+16, y,1, BRRED, BLACK);
	LCD_Chinese16ForFile(x+32, y,2, BRRED, BLACK);
	LCD_Chinese16ForFile(x+48, y,3, BRRED, BLACK);
	LCD_Chinese16ForFile(x+70, y,22, BRRED, BLACK);
	LCD_Chinese16ForFile(x+86, y,23, BRRED, BLACK);	
	LCD_Chinese16ForFile(x+102, y,5, BRRED, BLACK);
}
void LCD_CHINSES_GetPassWord(u8 x, u8 y)
{
	LCD_Chinese16ForFile(x, y,24, WHITE, BLACK);
	LCD_Chinese16ForFile(x+16, y,25, WHITE, BLACK);
	LCD_Chinese16ForFile(x+32, y,26, WHITE, BLACK);//��ѡ��
	
	LCD_Chinese16ForFile(x, y+20,10, WHITE, BLACK);
	LCD_Chinese16ForFile(x+16, y+20,11, WHITE, BLACK);
	LCD_Chinese16ForFile(x+32, y+20,12, WHITE, BLACK);//ָ��ʶ�� ��
	LCD_Chinese16ForFile(x+48, y+20,13, WHITE, BLACK);
	LCD_Chinese16ForFile(x+66, y+20,27, WHITE, BLACK);
	
	LCD_String(x+83,y+20,"*",16, RED , BLACK);//*
	
	LCD_String(x,y+40,"or",16, RED , BLACK);
	
		LCD_Chinese16ForFile(x, y+40,18, WHITE, BLACK);
	LCD_Chinese16ForFile(x+16, y+40,19, WHITE, BLACK);
	LCD_Chinese16ForFile(x+32, y+40,16, WHITE, BLACK);//�������� ��
	LCD_Chinese16ForFile(x+48, y+40,17, WHITE, BLACK);
	LCD_Chinese16ForFile(x+66, y+40,27, WHITE, BLACK);
	
		LCD_String(x+83,y+40,"#",16, RED , BLACK);//#
//	LCD_Chinese16ForFile(x, y,24, WHITE, BLACK);
//	LCD_Chinese16ForFile(x+16, y,25, WHITE, BLACK);
//	LCD_Chinese16ForFile(x+32, y,26, WHITE, BLACK);//��ѡ��
	
	//LCD_Chinese16ForFile(x+64, y,3, WHITE, BLACK);	
}



