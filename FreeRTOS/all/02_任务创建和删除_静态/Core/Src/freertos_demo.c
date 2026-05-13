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
StackType_t start_task_stack[START_TASK_STACK_DEPTH];
StaticTask_t start_task_tcb;
void Start_Task(void *pvParameters);

/* Task1 任务 配置 */
#define TASK1_PRIORITY 2
#define TASK1_STACK_DEPTH 128
TaskHandle_t task1_handler;
StackType_t task1_stack[TASK1_STACK_DEPTH];
StaticTask_t task1_tcb;
void Task1(void *pvParameters);

/* Task2 任务 配置 */
#define TASK2_PRIORITY 3
#define TASK2_STACK_DEPTH 128
TaskHandle_t task2_handler;
StackType_t task2_stack[TASK2_STACK_DEPTH];
StaticTask_t task2_tcb;
void Task2(void *pvParameters);

/* Task3 任务 配置 */
#define TASK3_PRIORITY 4
#define TASK3_STACK_DEPTH 128
TaskHandle_t task3_handler;
StackType_t task3_stack[TASK3_STACK_DEPTH];
StaticTask_t task3_tcb;
void Task3(void *pvParameters);

/* 空闲任务配置 */
StaticTask_t idle_task_tcb;
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];

/* 软件定时器任务配置 */
StaticTask_t timer_task_tcb;
StackType_t timer_task_stack[configTIMER_TASK_STACK_DEPTH];

/* 空闲任务内存分配 */
void vApplicationGetIdleTaskMemory(StaticTask_t **ppxIdleTaskTCBBuffer,
                                   StackType_t **ppxIdleTaskStackBuffer,
                                   uint32_t *pulIdleTaskStackSize)
{
    *ppxIdleTaskTCBBuffer = &idle_task_tcb;
    *ppxIdleTaskStackBuffer = idle_task_stack;
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

/* 软件定时器内存分配 */
void vApplicationGetTimerTaskMemory(StaticTask_t **ppxTimerTaskTCBBuffer,
                                    StackType_t **ppxTimerTaskStackBuffer,
                                    uint32_t *pulTimerTaskStackSize)
{
    *ppxTimerTaskTCBBuffer = &timer_task_tcb;
    *ppxTimerTaskStackBuffer = timer_task_stack;
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
}

/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
void FreeRTOS_Start(void)
{

    start_task_handler = xTaskCreateStatic((TaskFunction_t)Start_Task,
                                           (char *)"Start_Task",
                                           (uint32_t)START_TASK_STACK_DEPTH,
                                           (void *)NULL,
                                           (UBaseType_t)START_TASK_PRIORITY,
                                           (StackType_t *)start_task_stack,
                                           (StaticTask_t *)&start_task_tcb);
    vTaskStartScheduler();
}

void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* 进入临界区 */

    task1_handler = xTaskCreateStatic((TaskFunction_t)Task1,
                                      (char *)"Task1",
                                      (uint32_t)TASK1_STACK_DEPTH,
                                      (void *)NULL,
                                      (UBaseType_t)TASK1_PRIORITY,
                                      (StackType_t *)task1_stack,
                                      (StaticTask_t *)&task1_tcb);

    task2_handler = xTaskCreateStatic((TaskFunction_t)Task2,
                                      (char *)"Task2",
                                      (uint32_t)TASK2_STACK_DEPTH,
                                      (void *)NULL,
                                      (UBaseType_t)TASK2_PRIORITY,
                                      (StackType_t *)task2_stack,
                                      (StaticTask_t *)&task2_tcb);

    task3_handler = xTaskCreateStatic((TaskFunction_t)Task3,
                                      (char *)"Task3",
                                      (uint32_t)TASK3_STACK_DEPTH,
                                      (void *)NULL,
                                      (UBaseType_t)TASK3_PRIORITY,
                                      (StackType_t *)task3_stack,
                                      (StaticTask_t *)&task3_tcb);

    vTaskDelete(start_task_handler);  /* 创建完3个Task，删除Start任务，避免重复创建 */

    taskEXIT_CRITICAL(); /* 退出临界区 */
}

/**
 * @description: LED1每500ms翻转一次
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void *pvParameters)
{
    while (1)
    {
        printf("task1运行....\r\n");
        LED_Toggle(LED1_Pin);
        vTaskDelay(500);
    }
}

/**
 * @description: LED2每500ms翻转一次
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void *pvParameters)
{
    while (1)
    {
        printf("task2运行....\r\n");
        LED_Toggle(LED2_Pin);
        vTaskDelay(500);
    }
}

/**
 * @description: 按下KEY1删除task1
 * @param {void *} pvParameters
 * @return {*}
 */
void Task3(void *pvParameters)
{
    uint8_t key = 0;
    while (1)
    {
        printf("task3正在运行...\r\n");
        key = Key_Detect();
        if (key == KEY1_PRESS)
        {
            if (task1_handler != NULL)
            {
                printf("删除task1任务...\r\n");
                vTaskDelete(task1_handler);
                task1_handler = NULL;
            }
        }
        vTaskDelay(10);
    }
}
