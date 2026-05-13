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
#define TASK2_PRIORITY 2
#define TASK2_STACK_DEPTH 128
TaskHandle_t task2_handler;
void Task2(void *pvParameters);


/**
 * @description: for循环实现延时函数
 * @param {uint32_t} ms
 * @return {*}
 */
void for_delay_ms(uint32_t ms)
{
    uint32_t Delay = ms * 72000/9; /* 72M时钟频率，9是PLL倍频 */
    do
    {
        __NOP(); /* 空指令（NOP）来占用 CPU 时间 */
    }
    while (Delay --);
}


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
                
    xTaskCreate((TaskFunction_t         )   Task2,
                (char *                 )   "Task2",
                (configSTACK_DEPTH_TYPE )   TASK2_STACK_DEPTH,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK2_PRIORITY,
                (TaskHandle_t *         )   &task2_handler );
                

    vTaskDelete(NULL);                  /* 创建完其他Task，删除Start任务，避免重复创建 */
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/**
 * @description: 打印任务1执行次数
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void * pvParameters)
{
    uint16_t task1_count=0;
    while(1)
    {
        /* 临界区避免printf执行一半被打断 */
        taskENTER_CRITICAL();
        printf("task1运行次数=%d..\r\n",++task1_count);
        for_delay_ms(10);
        taskEXIT_CRITICAL();
    }
}

/**
 * @description: 打印任务2执行次数
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void * pvParameters)
{
    uint16_t task2_count=0;
    while(1)
    {
        /* 临界区避免printf执行一半被打断 */
        taskENTER_CRITICAL();
        printf("task2运行次数=%d..\r\n",++task2_count);
        for_delay_ms(10);
        taskEXIT_CRITICAL();
    }
}


