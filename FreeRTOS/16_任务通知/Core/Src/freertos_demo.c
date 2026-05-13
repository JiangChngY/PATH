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

    vTaskDelete(NULL);                  /* 创建完3个Task，删除Start任务，避免重复创建 */
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/**
 * @description: ?用于按键扫描，当检测到按键KEY1被按下时，将发送任务通知。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void * pvParameters)
{
    uint8_t key = 0;
    BaseType_t res = 0 ;
    while(1)
    {
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            //发送任务通知
            res = xTaskNotifyGive(task2_handler);
            if(res == pdPASS)
            {
                printf("task1向task2发送任务成功\r\n");
            }
        }
        vTaskDelay(500);
    }
}

/**
 * @description: ?用于接收任务通知，并打印相关提示信息。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void * pvParameters)
{
    uint32_t notifyValue = 0;
    while(1)
    {
        printf("task2运行\r\n");
        notifyValue = ulTaskNotifyTake(
            pdTRUE,             //接收完通知后，是否对通知清零，Y。0   F-1
            portMAX_DELAY       //等待任务通知的阻塞时间（M）
            );
        printf("task2接收到的通知值=%d\r\n" , notifyValue);
        vTaskDelay(500);
    }
}



