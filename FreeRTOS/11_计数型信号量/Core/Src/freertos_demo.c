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

 

/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
 QueueHandle_t cemCountHandle;
void FreeRTOS_Start(void)
{
    UBaseType_t count = 0;
    //创建一个计数型信号量
    xSemaphoreCreateCounting(100,0);
    if(cemCountHandle == NULL )
    {
        printf("二值信号量创建失败");
    }
    else{
        //创建成功获取一次计数值,并打印
        count = uxSemaphoreGetCount(cemCountHandle);
        printf("创建成功，初始计数值=%d\r\n", count);
        
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
 
    vTaskDelete(NULL);                  /* 创建完3个Task，删除Start任务，避免重复创建 */
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/**
 * @description: ?用于按键扫描，当检测到按键KEY1被按下时，释放计数型信号量。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void * pvParameters)
{
    uint8_t key;
    BaseType_t res;
    while(1)
    {
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            //释放二值信号量，计数值加一
            res = xSemaphoreGive(cemCountHandle);
            if(res == pdPASS)
            {
                printf("Task1释放计数成功\r\n");
            }
            else{
                printf("释放失败");
            }
        }
        vTaskDelay(500);
    }
}

/**
 * @description: ?每过一秒获取一次计数型信号量，当成功获取后打印信号量计数值。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void * pvParameters)
{
    BaseType_t res = 0;
    while(1)
    {
        UBaseType_t currentCount = 0;

        //获取二值信号量
        xSemaphoreTake(cemCountHandle , portMAX_DELAY);
        
        if(res == pdPASS)
        {
            printf("task2获取信号量成功");
        }
        else{
            printf("获取信号量失败");
        }
        currentCount = uxSemaphoreGetCount(cemCountHandle);
        printf("创建成功，当前计数值=%d\r\n", currentCount);
        
        vTaskDelay(1000);
    }
}

 

