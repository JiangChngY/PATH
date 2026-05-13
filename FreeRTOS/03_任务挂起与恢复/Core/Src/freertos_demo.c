#include "freertos_demo.h"
/* freertos相关的头文件 */
#include "FreeRTOS.h"
#include "task.h"
/* 任务需要用到 */
#include "LED.h"
#include "Key.h"
#include "usart.h"

/* 启动任务的参数配置 */
#define START_STACK_DEPTH 128
#define START_TASK_PRIORITY 1
TaskHandle_t start_task_handler;
void Start_Task(void *pvParameters);

/* 任务1的参数配置 */
#define TASK1_STACK_DEPTH 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handler;
void Task1(void *pvParameters);

/* 任务2的参数配置 */
#define TASK2_STACK_DEPTH 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handler;
void Task2(void *pvParameters);

/* 任务3的参数配置 */
#define TASK3_STACK_DEPTH 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handler;
void Task3(void *pvParameters);

/**
 * @description: 入口函数：创建启动任务、启动调度器
 * @return {*}
 */
void FreeRTOS_Start()
{
    /* 1、创建启动任务 */
    xTaskCreate(
        (TaskFunction_t)Start_Task,
        (char *)"Start_Task",
        (configSTACK_DEPTH_TYPE)START_STACK_DEPTH,
        (void *)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (TaskHandle_t *)&start_task_handler);

    /* 2、启动调度器 */
    // xPortStartScheduler();
    vTaskStartScheduler();
}

/**
 * @description: 启动任务：创建其他任务
 * @return {*}
 */
void Start_Task(void *pvParameters)
{
    /* 进入临界区 */
    // vPortEnterCritical();
    taskENTER_CRITICAL();

    /* 创建任务1 */
    xTaskCreate(
        (TaskFunction_t)Task1,
        (char *)"Task1",
        (configSTACK_DEPTH_TYPE)TASK1_STACK_DEPTH,
        (void *)NULL,
        (UBaseType_t)TASK1_PRIORITY,
        (TaskHandle_t *)&task1_handler);
    /* 创建任务2 */
    xTaskCreate(
        (TaskFunction_t)Task2,
        (char *)"Task2",
        (configSTACK_DEPTH_TYPE)TASK2_STACK_DEPTH,
        (void *)NULL,
        (UBaseType_t)TASK2_PRIORITY,
        (TaskHandle_t *)&task2_handler);
    /* 创建任务3 */
    xTaskCreate(
        (TaskFunction_t)Task3,
        (char *)"Task3",
        (configSTACK_DEPTH_TYPE)TASK3_STACK_DEPTH,
        (void *)NULL,
        (UBaseType_t)TASK3_PRIORITY,
        (TaskHandle_t *)&task3_handler);

    /* 启动任务只需要执行一次即可，用完就删 */
    vTaskDelete(NULL);

    /* 退出临界区 */
    // vPortExitCritical();
    taskEXIT_CRITICAL();
}

/**
 * @description: 任务1：LED1每500ms翻转一次
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
 * @description: 任务2：LED2每500ms翻转一次
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
 * @description: 任务3：?判断按键按下逻辑，KEY1按下，挂起task1，按下KEY2在任务中恢复task1。以及挂起调度器
 * @return {*}
 */
void Task3(void *pvParameters)
{
    uint8_t key = 0;
    char task_info[500];//接收任务信息字符串
    while (1)
    {
        printf("task3运行....\r\n");
        key = Key_Detect();

        if (key == KEY1_PRESS)
        {
           //KEY按下挂起task1
            printf("挂起task1...\r\n");
           vTaskSuspend(task1_handler);
        }
        else if(key == KEY2_PRESS)
        {
            //key2 按下 ，恢复task1
            printf("恢复task1...\r\n");
            vTaskResume(task1_handler);
        }
        else if(key == KEY3_PRESS)
        {
            printf("挂起调度器..\r\n");

            //key3 按下 ，挂起调度器
            vTaskSuspendAll();
        }
        else if(key == KEY4_PRESS)
        {
            printf("恢复调度器...\r\n");

            //key4 按下 ，恢复调度器
            xTaskResumeAll();
        }
        //打印任务状态
        vTaskList(task_info);
        printf("%s\r\n",task_info);
        
        vTaskDelay(500);  //调用FreeRTOS的时间延迟函数，才能进入阻塞态
    }
}
