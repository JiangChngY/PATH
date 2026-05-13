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

/* Task2 任务 配置 */
#define TASK2_PRIORITY 3
#define TASK2_STACK_DEPTH 128
TaskHandle_t task2_handler;
void Task2(void *pvParameters);

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
 * @description: 发送任务通知值
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void *pvParameters)
{
    uint8_t key = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            printf("将bit0位置1\r\n");
            xTaskNotify(task2_handler, EVENTBIT_0, eSetBits);
        }
        else if (key == KEY2_PRESS)
        {
            printf("将bit1位置1\r\n");
            xTaskNotify(task2_handler, EVENTBIT_1, eSetBits);
        }
        vTaskDelay(10);
    }
}

/**
 * @description: 接收任务通知值
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void *pvParameters)
{
    uint32_t notify_val = 0,event_bit = 0;
    while(1)
    {
        xTaskNotifyWait( 0, 0xFFFFFFFF, &notify_val, portMAX_DELAY );
        if(notify_val & EVENTBIT_0)
        {
            event_bit |= EVENTBIT_0;
        }
        if(notify_val & EVENTBIT_1)
        {
            event_bit |= EVENTBIT_1;
        }
        if(event_bit == (EVENTBIT_0|EVENTBIT_1))
        {
            printf("任务通知模拟事件标志组接收成功\r\n");
            event_bit = 0;
        }
    }
}
