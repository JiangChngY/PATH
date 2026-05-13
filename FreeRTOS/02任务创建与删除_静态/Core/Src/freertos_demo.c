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
TaskHandle_t start_task_handle;

StackType_t start_task_stack[START_STACK_DEPTH];//静态任务的任务栈
StaticTask_t satrt_task_tcb;//静态任务的TCB结构体

void Start_Task(void *pvParameters);



/* 任务1的参数配置 */
#define TASK1_STACK_DEPTH 128
#define TASK1_PRIORITY 2
TaskHandle_t task1_handler;
StackType_t task1_stack[TASK1_STACK_DEPTH];//静态任务的任务栈
StaticTask_t task1_tcb;//静态任务的TCB结构体

void Task1(void *pvParameters);

/* 任务2的参数配置 */
#define TASK2_STACK_DEPTH 128
#define TASK2_PRIORITY 3
TaskHandle_t task2_handler;
StackType_t task2_stack[TASK2_STACK_DEPTH];//静态任务的任务栈
StaticTask_t task2_tcb;//静态任务的TCB结构体
void Task2(void *pvParameters);

/* 任务3的参数配置 */
#define TASK3_STACK_DEPTH 128
#define TASK3_PRIORITY 4
TaskHandle_t task3_handler;
StackType_t task3_stack[TASK3_STACK_DEPTH];//静态任务的任务栈
StaticTask_t task3_tcb;//静态任务的TCB结构体
void Task3(void *pvParameters);


/*=======静态创建方式，需要两个手动指定两个特殊任务的资源========*/
/*空闲任务的配置*/
StackType_t idle_task_stack[configMINIMAL_STACK_SIZE];
StaticTask_t idle_task_tcb;

/*软件定时器任务的配置*/
StackType_t timer_task_stack[configMINIMAL_STACK_SIZE];
StaticTask_t timer_task_tcb;


void vApplicationGetIdleTaskMemory( StaticTask_t ** ppxIdleTaskTCBBuffer,
                                        StackType_t ** ppxIdleTaskStackBuffer,
                                        uint32_t * pulIdleTaskStackSize )
{
    * ppxIdleTaskTCBBuffer = &idle_task_tcb;
    * ppxIdleTaskStackBuffer = idle_task_stack;
    * pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
}

void vApplicationGetTimerTaskMemory( StaticTask_t ** ppxTimerTaskTCBBuffer,
                                         StackType_t ** ppxTimerTaskStackBuffer,
                                         uint32_t * pulTimerTaskStackSize )
{
    * ppxTimerTaskTCBBuffer = &timer_task_tcb;
    * ppxTimerTaskStackBuffer = timer_task_stack;
    * pulTimerTaskStackSize = configMINIMAL_STACK_SIZE;
}

/**
 * @description: 入口函数：创建启动任务、启动调度器
 * @return {*}
 */
void FreeRTOS_Start()
{
    start_task_handle = xTaskCreateStatic(
        (TaskFunction_t)Start_Task,
        (char *)"Start_Task",
        (configRUN_TIME_COUNTER_TYPE)START_STACK_DEPTH,
        (void *)NULL,
        (UBaseType_t)START_TASK_PRIORITY,
        (StackType_t *)start_task_stack,//任务栈(数组)的地址
        (StaticTask_t *)&satrt_task_tcb//tcb的地址
     );

    /* 2、启动调度器
       和软件定时器     */
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

        task1_handler = xTaskCreateStatic(
        (TaskFunction_t)Task1,
        (char *)"Task1",
        (configSTACK_DEPTH_TYPE)TASK1_STACK_DEPTH,
        (void *)NULL,
        (UBaseType_t)TASK1_PRIORITY,
        (StackType_t *)task1_stack,//任务栈(数组)的地址
        (StaticTask_t *)&task1_tcb//tcb的地址
     );
        
    /* 创建任务2 */
        task2_handler = xTaskCreateStatic(
        (TaskFunction_t)Task2,
        (char *)"Task2",
        (configSTACK_DEPTH_TYPE)TASK2_STACK_DEPTH,
        (void *)NULL,
        (UBaseType_t)TASK2_PRIORITY,
        (StackType_t *)task2_stack,//任务栈(数组)的地址
        (StaticTask_t *)&task2_tcb//tcb的地址
     );
        
    /* 创建任务3 */
        task3_handler = xTaskCreateStatic(
        (TaskFunction_t)Task3,
        (char *)"Task3",
        (configSTACK_DEPTH_TYPE)TASK3_STACK_DEPTH,
        (void *)NULL,
        (UBaseType_t)TASK3_PRIORITY,
        (StackType_t *)task3_stack,//任务栈(数组)的地址
        (StaticTask_t *)&task3_tcb//tcb的地址
     );

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
 * @description: 任务3：KEY1按下，删除Task1
 * @return {*}
 */
void Task3(void *pvParameters)
{
    uint8_t key = 0;
    while (1)
    {
        printf("task3运行....\r\n");
        key = Key_Detect();

        /* 测试阻塞后，是否接着运行 */
        // vTaskDelay(500);  //调用FreeRTOS的时间延迟函数，才能进入阻塞态
        // printf("delay后....\r\n");

        if (key == KEY1_PRESS)
        {
            /* 判断是否已经删过,避免重复执行删除 */
            if (task1_handler != NULL)
            {
                printf("执行删除Task1.....\r\n");
                vTaskDelete(task1_handler);
                task1_handler = NULL;
            }
        }
        vTaskDelay(500);  //调用FreeRTOS的时间延迟函数，才能进入阻塞态
        // HAL_Delay(500);   //HAL库和自定义的延时函数，不会阻塞任务
    }
}
