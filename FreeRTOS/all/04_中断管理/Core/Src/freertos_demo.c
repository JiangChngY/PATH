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

void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* 进入临界区 */
    xTaskCreate((TaskFunction_t)Task1,
                (char *)"Task1",
                (configSTACK_DEPTH_TYPE)TASK1_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK1_PRIORITY,
                (TaskHandle_t *)&task1_handler);

    vTaskDelete(NULL);   /* 创建完3个Task，删除Start任务，避免重复创建 */
    taskEXIT_CRITICAL(); /* 退出临界区 */
}

/**
 * @description: 开关中断
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void *pvParameters)
{
    uint8_t count = 0;
    uint32_t delay = 0;
    while (1)
    {
        delay=500000;
        if(++count == 5)
        {
            count = 0;
            printf("执行关中断...\r\n");
            portDISABLE_INTERRUPTS();
            while(delay--);
            printf("执行开中断...\r\n");
            portENABLE_INTERRUPTS();
        }
        vTaskDelay(1000);
    }
}
