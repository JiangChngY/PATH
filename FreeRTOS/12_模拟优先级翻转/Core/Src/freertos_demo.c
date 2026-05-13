#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "LED.h"
#include "Key.h"
#include "semphr.h"//信号量相关的头文件

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
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
 QueueHandle_t semHandle;
void FreeRTOS_Start(void)
{
    //创建一个二值信号量
//    xSemaphoreCreateBinary //x开头不会主动释放信号量，v可以主动释放一次
    vSemaphoreCreateBinary(semHandle);
    if(semHandle == NULL )
    {
        printf("二值信号量创建失败");
        
    }
    
    
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
                
    xTaskCreate((TaskFunction_t         )   Task3,
                (char *                 )   "Task3",
                (configSTACK_DEPTH_TYPE )   TASK3_STACK_DEPTH,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK3_PRIORITY,
                (TaskHandle_t *         )   &task3_handler );
 
    vTaskDelete(NULL);                  /* 创建完3个Task，删除Start任务，避免重复创建 */
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/**
 * @description: ?用于按键扫描，当检测到按键KEY1被按下时，释放二值信号量。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void * pvParameters)
{
    BaseType_t res;
    while(1)
    {
        //获取信号量
        printf("低优先级的task1获取信号量\r\n");
        res = xSemaphoreTake(semHandle , portMAX_DELAY);
        if(res != pdPASS)
        {
            printf("task1获取信号量失败\r\n");
        }
        //执行其他逻辑
        printf("低优先级的task3释放信号量");
        HAL_Delay(3000);
        //释放信号量
            
        res = xSemaphoreGive(semHandle);
        if(res != pdPASS)
        {
            printf("Task1 释放失败 \r\n");
        }

        vTaskDelay(1000);
    }
}

/**
 * @description: 获取二值信号量，当成功获取后打印提示信息
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void * pvParameters)
{
    BaseType_t res = 0;
    while(1)
    {
        printf("中优先级的task2正在执行");
        HAL_Delay(1500);
        printf("task2完成执行一次");
        vTaskDelay(1000);
    }
}

/**
 * @description: 获取二值信号量，当成功获取后打印提示信息
 * @param {void *} pvParameters
 * @return {*}
 */
void Task3(void * pvParameters)
{
    BaseType_t res = 0;
    while(1)
    {
        //获取信号量
        printf("高优先级的task3获取信号量\r\n");
        res = xSemaphoreTake(semHandle , portMAX_DELAY);
        if(res != pdPASS)
        {
            printf("task3获取信号量失败\r\n");
        }
        //执行其他逻辑
        printf("高优先级的task3释放信号量");
        HAL_Delay(1000);
        //释放信号量
            
        res = xSemaphoreGive(semHandle);
        if(res != pdPASS)
        {
            printf("Task3 释放失败 \r\n");
        }

        vTaskDelay(1000);
    }
}

