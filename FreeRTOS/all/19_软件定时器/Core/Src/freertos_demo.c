#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "timers.h"
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

void timer1_callback(TimerHandle_t pxTimer);
void timer2_callback(TimerHandle_t pxTimer);

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

TimerHandle_t timer1_handle = 0; /* 单次定时器 */
TimerHandle_t timer2_handle = 0; /* 周期定时器 */
void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* 进入临界区 */
    /* 创建单次定时器 */
    timer1_handle = xTimerCreate("timer1",
                                 500,
                                 pdFALSE,
                                 (void *)1,
                                 timer1_callback);

    /* 创建周期定时器 */
    timer2_handle = xTimerCreate("timer2",
                                 2000,
                                 pdTRUE,
                                 (void *)2,
                                 timer2_callback);

    xTaskCreate((TaskFunction_t)Task1,
                (char *)"Task1",
                (configSTACK_DEPTH_TYPE)TASK1_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK1_PRIORITY,
                (TaskHandle_t *)&task1_handler);
    vTaskDelete(NULL);
    taskEXIT_CRITICAL(); /* 退出临界区 */
}

/**
 * @description: 根据按键控制软件定时器
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
            xTimerStart(timer1_handle, portMAX_DELAY);
            xTimerStart(timer2_handle, portMAX_DELAY);
        }
        else if (key == KEY2_PRESS)
        {
            xTimerStop(timer1_handle, portMAX_DELAY);
            xTimerStop(timer2_handle, portMAX_DELAY);
        }
        vTaskDelay(10);
    }
}

/**
 * @description: timer1的超时回调函数
 * @param {TimerHandle_t} pxTimer 定时器句柄
 * @return {*}
 */
void timer1_callback(TimerHandle_t pxTimer)
{
    static uint32_t timer = 0;
    printf("timer1的运行次数=%d\r\n", ++timer);
}

/**
 * @description: timer2的超时回调函数
 * @param {TimerHandle_t} pxTimer 定时器句柄
 * @return {*}
 */
void timer2_callback(TimerHandle_t pxTimer)
{
    static uint32_t timer = 0;
    printf("timer2的运行次数=%d\r\n", ++timer);
}
