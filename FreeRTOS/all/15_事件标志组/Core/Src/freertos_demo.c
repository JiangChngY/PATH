#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "event_groups.h"
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

/* Task2 任务 配置 */
#define TASK2_PRIORITY 3
#define TASK2_STACK_DEPTH 128
TaskHandle_t task2_handler;
void Task2(void *pvParameters);


EventGroupHandle_t  eventgroup_handle;
#define EVENTBIT_0  (1 << 0)
#define EVENTBIT_1  (1 << 1)
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
    /* 创建事件标志组 */
    eventgroup_handle = xEventGroupCreate();
    if(eventgroup_handle != NULL)
    {
        printf("事件标志组创建成功\r\n");
    }
    xTaskCreate((TaskFunction_t         )   Task1,
                (char *                 )   "Task1",
                (configSTACK_DEPTH_TYPE )   TASK1_STACK_DEPTH,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK1_PRIORITY,
                (TaskHandle_t *         )   &task1_handler );
                
    xTaskCreate((TaskFunction_t         )   Task2,
                (char *                 )   "Task2",
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_DEPTH,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIORITY,
                (TaskHandle_t *         )   &task2_handler );
    vTaskDelete(NULL);                  
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/**
 * @description: 根据按键，事件标志组相应为置一
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void * pvParameters)
{
    uint8_t key = 0;
    while(1) 
    {
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            /* 将事件标志组的bit0位置1 */
            xEventGroupSetBits( eventgroup_handle, EVENTBIT_0); 
        }
        else if(key == KEY2_PRESS)
        {
            /* 将事件标志组的bit1位置1 */
            xEventGroupSetBits( eventgroup_handle, EVENTBIT_1); 
        }
        vTaskDelay(10);
    }
}

/**
 * @description: 同时等待事件标志组中的多个事件位
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void * pvParameters)
{
    EventBits_t event_bit = 0;
    while(1)
    {
        event_bit = xEventGroupWaitBits( eventgroup_handle,         /* 事件标志组句柄 */
                                         EVENTBIT_0 | EVENTBIT_1,   /* 等待事件标志组的bit0和bit1位 */
                                         pdTRUE,                    /* 成功等待到事件标志位后，清除事件标志组中的bit0和bit1位 */
                                         pdTRUE,                    /* 等待事件标志组的bit0和bit1位都置1,就成立 */
                                         portMAX_DELAY );           /* 一直等 */
        printf("等待到的事件标志位值=%#x\r\n",event_bit);
    }
}

