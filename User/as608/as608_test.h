#ifndef __AS608_TEST_H
#define	__AS608_TEST_H


#include "./as608/bsp_as608.h"


void Connect_Test(void);
void AS608_USART_Config(void);
uint16_t PS_Connect(uint32_t *PS_Addr);
uint16_t PS_GetImage(void);
void Add_FR(void);
void Compare_FR(void);
void Compare_FR_MQTT(char * Tips) ;
void Del_FR(void);
void Clean_FR(void);

void Show_Message(void);
void FR_Task(void);
void lcd_as608UIsomething(void);
void lcd_as608UInothing(void);
//void Mqtt_Task(char * Tips, int Cmd, uint16_t ID);
//void MQTT_Add_FR(char *Tips , uint16_t ID);
//void Del_FR_MQTT(char *Tips , uint16_t id);
//void Door_ID_Open(char *Tips,int State);
#endif /* __AS608_TEST_H */
