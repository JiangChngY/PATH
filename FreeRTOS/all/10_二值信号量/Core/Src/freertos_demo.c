#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
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

QueueHandle_t semphore_handle;
/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
void FreeRTOS_Start(void)
{
    semphore_handle = xSemaphoreCreateBinary();
    if (semphore_handle != NULL)
    {
        printf("二值信号量创建成功\r\n");
    }
    xTaskCreate((TaskFunction_t)Start_Task,
                (char *)"Start_Task",
                (configSTACK_DEPTH_TYPE)START_TASK_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)START_TASK_PRIORITY,
                (TaskHandle_t *)&start_task_handler);
    vTaskStartScheduler();
}

void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* 进入临界区 */
    xTaskCreate((TaskFunction_t)Task1,
                (char *)"Task1",
                (configSTACK_DEPTH_TYPE)TASK1_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK1_PRIORITY,
                (TaskHandle_t *)&task1_handler);

    xTaskCreate((TaskFunction_t)Task2,
                (char *)"Task2",
                (configSTACK_DEPTH_TYPE)TASK2_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK2_PRIORITY,
                (TaskHandle_t *)&task2_handler);
    vTaskDelete(NULL);
    taskEXIT_CRITICAL(); /* 退出临界区 */
}

/**
 * @description: 释放二值信号量
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void *pvParameters)
{
    uint8_t key = 0;
    BaseType_t err;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            if (semphore_handle != NULL)
            {
                err = xSemaphoreGive(semphore_handle);
                if (err == pdPASS)
                {
                    printf("信号量释放成功\r\n");
                }
                else
                    printf("信号量释放失败\r\n");
            }
        }
        vTaskDelay(10);
    }
}

/**
 * @description: 获取二值信号量
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void *pvParameters)
{
    uint32_t i = 0;
    BaseType_t err;
    while (1)
    {
        /* 一直等待获取信号量 */
        err = xSemaphoreTake(semphore_handle, portMAX_DELAY); 
        if (err == pdTRUE)
        {
            printf("获取信号量成功\r\n");
        }
        else
        {
            printf("已超时%d\r\n", ++i);
        }
    }
}
