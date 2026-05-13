#ifndef __KEY_H
#define __KEY_H

#include "main.h"

#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)   /* 读取KEY1引脚状态（上拉输入） */
#define KEY2        HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin)   /* 读取KEY2引脚状态（上拉输入） */
#define KEY3        HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin)   /* 读取KEY3引脚状态（下拉输入） */
#define KEY4        HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin)   /* 读取KEY4引脚状态（下拉输入） */


#define KEY1_PRESS    1              
#define KEY2_PRESS    2              
#define KEY3_PRESS    3              
#define KEY4_PRESS    4              


uint8_t Key_Detect(void);


#endif
