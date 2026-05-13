#include "freertos_demo.h"
#include "FreeRTOS.h"
#include "task.h"
#include "queue.h"
#include "usart.h"
#include "LED.h"
#include "Key.h"
#include "semphr.h"//信号量头文件


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


QueueHandle_t queue1;    /* 小数据句柄 */
QueueHandle_t semHandle; 
QueueSetHandle_t queueSetHandle;
/**
 * @description: FreeRTOS入口函数：创建任务函数并开始调度
 * @return {*}
 */
void FreeRTOS_Start(void)
{
    BaseType_t res = 0;
    /* 创建queue1队列 */
    queue1 = xQueueCreate(2, sizeof(uint8_t));
    if (queue1 != NULL)
    {
        printf("queue1队列创建成功\r\n");
    }
    else
    {
        printf("queue1队列创建失败\r\n");
    }
    /* 创建二值信号量，创建完不主动释放 */
     semHandle = xSemaphoreCreateBinary();

    
    if (semHandle != NULL)
    {
        printf("二值信号量创建成功\r\n");
    }
    else
    {
        printf("二值信号量创建失败\r\n");
    }
    //创建队列集
    queueSetHandle = xQueueCreateSet(2);
    if(queueSetHandle != NULL)
    {
        printf("创建队列集成功\r\n");
    }
    //将队列以及信号量添加到队列集
    res = xQueueAddToSet(queue1 , queueSetHandle);
    if(res == pdPASS)
    {
        printf("queue1添加到队列集成功\r\n");
    }
    
    res = xQueueAddToSet(semHandle , queueSetHandle);
    if(res == pdPASS)
    {
        printf("二值信号量添加到队列集成功\r\n");
    }

    xTaskCreate((TaskFunction_t)Start_Task,
                (char *)"Start_Task",
                (configSTACK_DEPTH_TYPE)START_TASK_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)START_TASK_PRIORITY,
                (TaskHandle_t *)&start_task_handler);
    vTaskStartScheduler();
}

void Start_Task(void *pvParameters)
{
    taskENTER_CRITICAL(); /* 进入临界区 */
    xTaskCreate((TaskFunction_t)Task1,
                (char *)"Task1",
                (configSTACK_DEPTH_TYPE)TASK1_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK1_PRIORITY,
                (TaskHandle_t *)&task1_handler);

    xTaskCreate((TaskFunction_t)Task2,
                (char *)"Task2",
                (configSTACK_DEPTH_TYPE)TASK2_STACK_DEPTH,
                (void *)NULL,
                (UBaseType_t)TASK2_PRIORITY,
                (TaskHandle_t *)&task2_handler);


    vTaskDelete(NULL);
    taskEXIT_CRITICAL(); /* 退出临界区 */
}

/**
 * @description: ?用于扫描按键，当KEY1按下，往队列写入数据，当KEY2按下，释放二值信号量。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void *pvParameters)
{
    uint8_t key = 0;
    uint8_t res = 0;

    while (1)
    {
        key = Key_Detect();
        if (key == KEY1_PRESS )
        {
            res = xQueueSend(queue1, &key, portMAX_DELAY);
            if (res  == pdPASS)
            {
                printf("往queue1发送数据[%d]成功\r\n" , key);
            }
            else{
            printf("send data defeat");
            }
        }
        else if (key == KEY2_PRESS)
        {
            //释放信号量
            res = xSemaphoreGive(semHandle);
            if (res == pdPASS)
            {
                printf("释放信号量成功\r\n");
            }
            else{
                printf("释放信号量失败\r\n");
            }
        }
        vTaskDelay(500);
    }
}

/**
 * @description: ?读取队列集中的消息，并打印。
 * @param {void *} pvParameters
 * @return {*}
 */
void Task2(void *pvParameters)
{
    QueueSetMemberHandle_t memberHandle;
    
    uint8_t key = 0;
    BaseType_t res = 0;
    uint8_t receive = 0;

    while (1)
    {
        //查看那个队列有数据来了
        memberHandle = xQueueSelectFromSet(queueSetHandle , portMAX_DELAY);
        //根据对应队列获取数据
        if(memberHandle == queue1)
        {

            res = xQueueReceive(queue1, &key, portMAX_DELAY);
            if (res == pdPASS)
            {
                printf("task2从queue1队列读取成功[%d]\r\n" , receive);
            }
            else
            {
                printf("queue1读取队列成功，数据：%d\r\n", key);
            }
        }
        else if(memberHandle == semHandle)
        {
            res = xSemaphoreTake(semHandle , portMAX_DELAY);
            if(res == pdPASS)
            {
                printf("获取信号量成功\r\n");
            }
            else{
                printf("从队列集获取信息失败\r\n");
            }
        }
        

    }
}

