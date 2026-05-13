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
// #define TASK2_STACK_DEPTH 105
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
 * @description: LED1每500ms翻转一次
 * @param {void *} pvParameters
 * @return {*}
 */
void Task1(void *pvParameters)
{
    while (1)
    {
        LED_Toggle(LED1_Pin);
        vTaskDelay(500);
    }
}

/**
 * @description: 查询任务信息
 * @param {void *} pvParameters
 * @return {*}
 */
char task_info[500];
void Task2(void *pvParameters)
{
    UBaseType_t task_priority = 0;
    UBaseType_t task_num = 0;
    UBaseType_t task_num2 = 0;
    TaskStatus_t task_status[4] = 0;
    TaskStatus_t task_status2[1] = 0;
    TaskHandle_t task_handle = 0;
    UBaseType_t task_stack_remain_min = 0; 
    eTaskState task_state = 0;

    /* 查询任务优先级 */
    task_priority = uxTaskPriorityGet(task1_handler);
    printf("task1任务优先级=%d....\r\n", task_priority);
    task_priority = uxTaskPriorityGet(task2_handler);
    printf("task2任务优先级=%d....\r\n", task_priority);

    /* 设置任务优先级 */
    vTaskPrioritySet(task1_handler, 4);
    task_priority = uxTaskPriorityGet(task1_handler);
    printf("task1任务优先级=%d....\r\n", task_priority);

    /* 查询任务数量：包含启动调度器时底层启动的任务 */
    task_num = uxTaskGetNumberOfTasks();
    printf("任务数量=%d....\r\n", task_num);

    /* 获取系统状态 */
    task_num2 = uxTaskGetSystemState(task_status, task_num, NULL);
    printf("任务名\t任务编号\t任务优先级\r\n");
    for (uint8_t i = 0; i < task_num2; i++)
    {
        printf("%s\t%d\t%d\r\n",
               task_status[i].pcTaskName,
               task_status[i].xTaskNumber,
               task_status[i].uxCurrentPriority);
    }

    /* 获取单个任务信息 */
    vTaskGetInfo(task1_handler,
                 task_status2,
                 pdTRUE,
                 eInvalid);
    printf("任务名：%s\r\n", task_status2->pcTaskName);
    printf("任务编号：%d\r\n", task_status2->xTaskNumber);
    printf("任务优先级：%d\r\n", task_status2->uxCurrentPriority);
    printf("任务状态：%d\r\n", task_status2->eCurrentState);

    /* 根据任务名获取任务句柄 */
    task_handle = xTaskGetHandle("Task1");
    printf("获取Task1任务句柄:%#x\r\n",task_handle);
    printf("Task1任务句柄:%#x\r\n",task1_handler);

    /* 获取指定任务的任务栈历史最小剩余值 */
    task_stack_remain_min = uxTaskGetStackHighWaterMark( task2_handler ); 
    printf("task2任务栈历史最小值=%d\r\n",task_stack_remain_min);

    /* 获取指定任务的状态 */
    task_state = eTaskGetState( task2_handler );
    printf("task2当前任务状态=%d\r\n",task_state);

    /* 以表格形式获取系统中任务的信息 */
    vTaskList(task_info);
    printf("%s\r\n",task_info);
    while (1)
    {
        vTaskDelay(100);
    }
}
