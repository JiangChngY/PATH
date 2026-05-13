#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
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

QueueSetHandle_t queueset_handle;
QueueHandle_t queue_handle;
QueueHandle_t semphr_handle;
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
    /* 创建队列集，可以存放2个队列 */
    queueset_handle = xQueueCreateSet(2);
    if (queueset_handle != NULL)
    {
        printf("队列集创建成功\r\n");
    }
    /* 创建队列 */
    queue_handle = xQueueCreate(1, sizeof(uint8_t));
    /* 创建二值信号量 */
    semphr_handle = xSemaphoreCreateBinary();
    /* 添加到队列集 */
    xQueueAddToSet(queue_handle, queueset_handle);
    xQueueAddToSet(semphr_handle, queueset_handle);

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
 * @description: 实现队列发送以及信号量释放
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void *pvParameters)
{
    uint8_t key = 0;
    BaseType_t err = 0;
    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            err = xQueueSend(queue_handle, &key, portMAX_DELAY);
            if (err == pdPASS)
            {
                printf("往队列queue_handle写入数据成功\r\n");
            }
        }
        else if (key == KEY2_PRESS)
        {
            err = xSemaphoreGive(semphr_handle);
            if (err == pdPASS)
            {
                printf("释放信号量成功\r\n");
            }
        }
        vTaskDelay(10);
    }
}

/**
 * @description: 获取队列集的消息
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void *pvParameters)
{
    QueueSetMemberHandle_t member_handle;
    uint8_t key;
    while (1)
    {
        member_handle = xQueueSelectFromSet(queueset_handle, portMAX_DELAY);
        if (member_handle == queue_handle)
        {
            xQueueReceive(member_handle, &key, portMAX_DELAY);
            printf("获取到的队列数据=%d\r\n", key);
        }
        else if (member_handle == semphr_handle)
        {
            xSemaphoreTake(member_handle, portMAX_DELAY);
            printf("获取信号量成功\r\n");
        }
    }
}

