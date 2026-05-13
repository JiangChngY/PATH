#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
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

/* Task3 任务 配置 */
#define TASK3_PRIORITY 4
#define TASK3_STACK_DEPTH 128
TaskHandle_t task3_handler;
void Task3(void *pvParameters);

QueueHandle_t queue1;    /* 小数据句柄 */
QueueHandle_t big_queue; /* 大数据句柄 */
char buff[100] = {"大大大fdahjk324hjkhfjksdahjk#$@!@#jfaskdfhjka"};
/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
void FreeRTOS_Start(void)
{
    /* 创建queue1队列 */
    queue1 = xQueueCreate(2, sizeof(uint8_t));
    if (queue1 != NULL)
    {
        printf("queue1队列创建成功\r\n");
    }
    else
    {
        printf("queue1队列创建失败\r\n");
    }
    /* 创建big_queue队列 */
    big_queue = xQueueCreate(1, sizeof(char *));
    if (big_queue != NULL)
    {
        printf("big_queue队列创建成功\r\n");
    }
    else
    {
        printf("big_queue队列创建失败\r\n");
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

    xTaskCreate((TaskFunction_t)Task3,
                (char *)"Task2",
                (configSTACK_DEPTH_TYPE)TASK3_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK3_PRIORITY,
                (TaskHandle_t *)&task3_handler);
    vTaskDelete(NULL);
    taskEXIT_CRITICAL(); /* 退出临界区 */
}

/**
 * @description: 入队
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void *pvParameters)
{
    uint8_t key = 0;
    char *buf;
    BaseType_t err = 0;
    buf = &buff[0];
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS || key == KEY2_PRESS)
        {
            err = xQueueSend(queue1, &key, portMAX_DELAY);
            if (err != pdTRUE)
            {
                printf("queue1队列发送失败\r\n");
            }
        }
        else if (key == KEY3_PRESS)
        {
            err = xQueueSend(big_queue, &buf, portMAX_DELAY);
            if (err != pdTRUE)
            {
                printf("big_queue队列发送失败\r\n");
            }
        }
        vTaskDelay(10);
    }
}

/**
 * @description: 小数据出队
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void *pvParameters)
{
    uint8_t key = 0;
    BaseType_t err = 0;
    while (1)
    {
        err = xQueueReceive(queue1, &key, portMAX_DELAY);
        if (err != pdTRUE)
        {
            printf("queue1队列读取失败\r\n");
        }
        else
        {
            printf("queue1读取队列成功，数据：%d\r\n", key);
        }
    }
}

/**
 * @description: 大数据出队
 * @param {void *} pvParameters
 * @return {*}
 */
void Task3(void *pvParameters)
{
    char *buf;
    BaseType_t err = 0;
    while (1)
    {
        err = xQueueReceive(big_queue, &buf, portMAX_DELAY);
        if (err != pdTRUE)
        {
            printf("big_queue队列读取失败\r\n");
        }
        else
        {
            printf("数据：%s\r\n", buf);
        }
    }
}
