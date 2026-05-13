# 02 静态创建与删除任务

## 项目简介
本工程演示使用静态内存创建 FreeRTOS 任务。任务栈和 TCB 由用户提前定义，同时为空闲任务和软件定时器任务提供静态内存回调。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `xTaskCreateStatic()` 创建任务。
- 理解 `StackType_t` 任务栈数组和 `StaticTask_t` TCB 的作用。
- 实现 `vApplicationGetIdleTaskMemory()` 和 `vApplicationGetTimerTaskMemory()`。
- 对比静态创建与动态创建对堆内存的依赖差异。

## 运行现象
Task1/Task2 周期翻转 LED。按下 KEY1 删除 Task1，用来观察静态创建的任务同样可以通过 `vTaskDelete()` 删除。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


