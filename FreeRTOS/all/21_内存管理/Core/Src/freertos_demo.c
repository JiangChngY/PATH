#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "LED.h"
#include "Key.h"

/* 启动任务函数 */
#define START_TASK_PRIORITY 1
#define START_TASK_STACK_DEPTH 128
TaskHandle_t start_task_handler;
void Start_Task(void *pvParameters);

/* Task1 任务 配置 */
#define TASK1_PRIORITY 2
#define TASK1_STACK_DEPTH 128
TaskHandle_t task1_handler;
void Task1(void *pvParameters);


/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
void FreeRTOS_Start(void)
{
    xTaskCreate((TaskFunction_t)Start_Task,
                (char *)"Start_Task",
                (configSTACK_DEPTH_TYPE)START_TASK_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)START_TASK_PRIORITY,
                (TaskHandle_t *)&start_task_handler);
    vTaskStartScheduler();
}

void Start_Task( void * pvParameters )
{
    taskENTER_CRITICAL();               /* 进入临界区 */
    xTaskCreate((TaskFunction_t         )   Task1,
                (char *                 )   "Task1",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_DEPTH,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIORITY,
                (TaskHandle_t *         )   &task1_handler );
    vTaskDelete(NULL);                  
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/**
 * @description: 申请及释放内存，并显示空闲内存大小
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void * pvParameters)
{
    uint8_t key = 0, t = 0;
    uint8_t * buf = NULL;
    while(1) 
    {
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            /* 申请内存 */
            buf = pvPortMalloc(30);                 
            if(buf != NULL)
            {
                printf("申请内存成功\r\n");
            }
            else 
            {
                printf("申请内存失败\r\n");
            }
        }
        else if(key == KEY2_PRESS)
        {
            if(buf != NULL)
            {
                /* 释放内存 */
                vPortFree(buf);                     
                printf("释放内存\r\n");
            }                
        }
        if(t++ > 50)
        {
            t = 0;
            printf("剩余的空闲内存大小=%d\r\n",xPortGetFreeHeapSize());
        }
        vTaskDelay(10);
    }
}

