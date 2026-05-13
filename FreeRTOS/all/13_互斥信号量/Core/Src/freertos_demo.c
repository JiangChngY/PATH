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

/* Task3 任务 配置 */
#define TASK3_PRIORITY 4
#define TASK3_STACK_DEPTH 128
TaskHandle_t task3_handler;
void Task3(void *pvParameters);

/**
 * @description: for循环实现延时函数
 * @param {uint32_t} ms
 * @return {*}
 */
void for_delay_ms(uint32_t ms)
{
    uint32_t Delay = ms * 72000 / 9; /* 72M时钟频率，9是PLL倍频 */
    do
    {
        __NOP(); /* 空指令（NOP）来占用 CPU 时间 */
    } while (Delay--);
}

QueueHandle_t mutex_semphore_handle;
/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
void FreeRTOS_Start(void)
{
    /* 创建互斥信号量，并且主动释放一次信号量 */
    mutex_semphore_handle = xSemaphoreCreateMutex();
    if (mutex_semphore_handle != NULL)
    {
        printf("互斥信号量创建成功\r\n");
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
 * @description: 低优先级任务
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void *pvParameters)
{
    while (1)
    {
        printf("低优先级Task1获取信号量\r\n");
        xSemaphoreTake(mutex_semphore_handle, portMAX_DELAY);
        printf("低优先级Task1正在运行\r\n");
        for_delay_ms(3000);
        printf("低优先级Task1释放信号量\r\n");
        xSemaphoreGive(mutex_semphore_handle);
        vTaskDelay(1000);
    }
}

/**
 * @description: 中优先级任务
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void *pvParameters)
{
    while (1)
    {
        printf("中优先级Task2正在运行\r\n");
        vTaskDelay(1000);
    }
}

/**
 * @description: 高优先级任务
 * @param {void *} pvParameters
 * @return {*}
 */
void Task3(void *pvParameters)
{
    while (1)
    {
        printf("高优先级Task3获取信号量\r\n");
        xSemaphoreTake(mutex_semphore_handle, portMAX_DELAY);
        printf("高优先级Task3正在运行\r\n");
        for_delay_ms(1000);
        printf("高优先级Task3释放信号量\r\n");
        xSemaphoreGive(mutex_semphore_handle);
        vTaskDelay(1000);
    }
}
