#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "LED.h"
#include "Key.h"
#include "event_groups.h" //时间标志组头文件

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
EventGroupHandle_t eventGroupHandle;
void FreeRTOS_Start(void)
{
    //创建事件标志组
    eventGroupHandle = xEventGroupCreate();
    if(eventGroupHandle != NULL)
    {
        printf("创建事件标志组成功》》》\r\n");
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
 * @description: ?读取按键按下键值，根据不同键值将事件标志组相应事件位置一，模拟事件发生。
 * @param {void *} pvParameters
 * @return {*}
 */

#define EVENTBIT_0 (1<<0)
#define EVENTBIT_1 (1<<1)

void Task1(void * pvParameters)
{
    uint8_t key = 0;
    while(1)
    {
        key = Key_Detect();
        if(key == KEY1_PRESS)
        {
            //key按下，对bit0置1
            xEventGroupSetBits(eventGroupHandle,EVENTBIT_0);
            printf("按下key1对bit0置1\r\n");
        }
        else if(key == KEY2_PRESS)
        {
            //key2按下，对bit1置1
            xEventGroupSetBits(eventGroupHandle,EVENTBIT_1);
            printf("按下key2对bit1置1\r\n");
        }
    
        vTaskDelay(500);
    }
}

/**
 * @description: ?同时等待事件标志组中的多个事件位，当这些事件位都置 1 的话就执行相应的处理。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void * pvParameters)
{
    EventBits_t eventBits = 0;
    while(1)
    {
        eventBits = xEventGroupWaitBits(
            eventGroupHandle,           //事件标志组的句柄
            EVENTBIT_0 |EVENTBIT_1,     //要等待的标志位，可以写多个
            pdTRUE,                     //满足等待条件，是否清零
            pdTRUE,                     //所有BIT位都为1，还是其中有一即可
            portMAX_DELAY               //
        );
        printf("task2接收到的事件标志组=%#x>>>\r\n",eventBits);
    }
}


