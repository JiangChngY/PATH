#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "usart.h"
#include "LED.h"
#include "Key.h"
#include "timers.h"  //软件定时器的头文件

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


/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
 //软件定时器的超时回调函数
void timer1Callback(TimerHandle_t xTimer );//单次
void timer2Callback(TimerHandle_t xTimer );//周期


//软件定时器的句柄
TimerHandle_t timer1Handle;
TimerHandle_t timer2Handle;

void FreeRTOS_Start(void)
{
    //创建一次性软件定时器
    timer1Handle = xTimerCreate(
        "violet_mio",
        (TickType_t)500,        //超时的时间 = 500个 RTOS时钟节拍（500ms）
        pdFALSE,                //是否自动重载  pdFALSE非周期性
        (void *)1,               //定时器编号唯一ID
        timer1Callback//回调函数
        );
    if(timer1Handle != NULL)
    {
        printf("timer1一次性定时器创建成功>>..\r\n");
    }
        
    //创建周期性软件定时器
    timer2Handle = xTimerCreate(
        "violet_cl",
        (TickType_t)1000,        //超时的时间 = 500个 RTOS时钟节拍（500ms）
        pdTRUE,                //是否自动重载  pdFALSE非周期性
        (void *)2,               //定时器编号唯一ID
        timer2Callback//回调函数
        );
    if(timer2Handle != NULL)
    {
        printf("timer2周期性定时器创建成功>>..\r\n");
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
                
    vTaskDelete(NULL);                  /* 创建完Task，删除Start任务，避免重复创建 */
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/**
 * @description: LED1每500ms翻转一次
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void * pvParameters)
{
    uint8_t key = 0;
    uint8_t res = 0;

    while(1)
    {
        key = Key_Detect();
        //软件定时器
             if(key == KEY1_PRESS)
             {
                res = xTimerStart(timer1Handle , portMAX_DELAY);
                if(res != pdFAIL)
                {
                    printf("timer1单次定时器启动成功\r\n");
                }
                res = xTimerStart(timer2Handle , portMAX_DELAY);
                if(res != pdFAIL)
                {
                    printf("timer2周期定时器启动成功\r\n");
                }
             }
        //停止软件定时器
        else if(key == KEY2_PRESS)
        {
            res = xTimerStop(timer1Handle , portMAX_DELAY);
            if(res != pdFAIL)
            {
                printf("timer1单次定时器停止成功\r\n");
            }
            res = xTimerStop(timer2Handle , portMAX_DELAY);
            if(res != pdFAIL)
            {
                printf("timer2周期定时器停止成功\r\n");
            }
        }

        vTaskDelay(500);
    }
}



void timer1Callback(TimerHandle_t xTimer )
{
    static uint16_t timer1Count;
    printf("timer1超时回调=%d次...\r\n",timer1Count++);
    
}

void timer2Callback(TimerHandle_t xTimer )
{
    static uint16_t timerCount;
    printf("timer2超时回调=%d次...\r\n",timerCount++);
}





























