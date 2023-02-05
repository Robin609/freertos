#ifndef _MATRIX_KEY_H_
#define _MATRIX_KEY_H_

#include "stm32f10x.h"
#include "RT_Delay.h"
#include "stdio.h"

void Matrix_ssKey_Pin_Init(void);
int Matrix_Key_Scan(void);

void get_pwd(u16 pwd);
void KEY_Init(void);
int key_scan(void);
void verify (void);
void change_pwd (void);

#endif

