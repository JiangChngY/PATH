#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"//队列相关的头文件

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






//队列句柄queue1
QueueHandle_t queue1;
QueueHandle_t bigQueue;

/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */

void FreeRTOS_Start(void)
{
    //创建需要的队列
    //创建一个KEY值编号的小队列queue1
    queue1 = xQueueCreate(2,sizeof(uint8_t));
    if(queue1 != NULL)
    {
        printf("queue1 创建成功\r\n");
    }
    else
    {
        printf("queue1创建失败\r\n");
    }
    
    
    //创建一个存放大数据的队列bigQueue,存放地址即可
    bigQueue = xQueueCreate(1,sizeof(char *));
    if(bigQueue != NULL)
    {
        printf("bigQueue 创建成功\r\n");
    }
    else
    {
        printf("bigQueue创建失败\r\n");
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
                (char *                 )   "Task2",
                (configSTACK_DEPTH_TYPE )   TASK3_STACK_DEPTH,
                (void *                 )   NULL,
                (UBaseType_t            )   TASK3_PRIORITY,
                (TaskHandle_t *         )   &task3_handler );
    vTaskDelete(NULL);                  /* 创建完3个Task，删除Start任务，避免重复创建 */
    taskEXIT_CRITICAL();                /* 退出临界区 */
}

/**
 * @description: 当按键key1或key2按下，将键值拷贝到队列queue1（入队）；
                 当按键key3按下，将传输大数据，这里拷贝大数据的地址到队列big_queue中。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void * pvParameters)
{
    char *bigData = "bhcdcjpowcu8wcnjw46cdckjbdhjbvhjbekbvjhfvbhj";
    uint8_t key = 0;
    uint8_t res = 0;

    while(1)
    {

        key = Key_Detect();
        if(key == KEY1_PRESS | key == KEY2_PRESS )
        {
            //将key编号写入queue1
            res = xQueueSend(queue1 , &key , portMAX_DELAY);
            if(res == pdPASS)
            {
                printf("往queue1发送数据%d成功\r\n",key);
            }
            else{
                printf("往queue1发送数据defeat\r\n");
            }
        }
        else if(key == KEY3_PRESS)
        {
            //将字符串的地址（大数据）发送到bigQueue
            res = xQueueSend(bigQueue , &key , portMAX_DELAY);
            if(res == pdPASS)
            {
                printf("往bigQueue发送数据成功\r\n");
            }
            else{
                printf("往bigQueue发送数据defeat\r\n");
            }
        }
        vTaskDelay(500);
    }
}

/**
 * @description: 读取队列queue1中的消息（出队），打印出接收到的键值
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void * pvParameters)
{
    uint16_t receive = 0;
    uint8_t res = 0;

    while(1)
    {
        res = xQueueReceive(queue1 , &receive , portMAX_DELAY);
            if(res == pdPASS)
            {
                printf("task成功从queue1读取数据%d成功\r\n",receive);
            }
            else{
                printf("task 从queue1读取数据失败\r\n");
            }
    }
}

/**
 * @description:从队列big_queue读取大数据地址，通过地址访问大数据。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task3(void * pvParameters)
{
    uint8_t key = 0;
    char *receiveBuff;
    uint8_t res = 0;

    while(1)
        res = xQueueReceive(bigQueue , &receiveBuff , portMAX_DELAY);
        if(res == pdPASS)
        {
            printf("task成功从bigQueue读取数据%s成功\r\n",receiveBuff);
        }
        else{
            printf("task 从bigQueue读取数据失败\r\n");
        }
    }



