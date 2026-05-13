# 07 获取任务运行时间

## 项目简介
本工程演示 FreeRTOS 任务运行时间统计功能。通过高频计数变量为 `vTaskGetRunTimeStats()` 提供计数源，串口输出各任务运行时间占比。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 启用 `configGENERATE_RUN_TIME_STATS`。
- 配置 `portCONFIGURE_TIMER_FOR_RUN_TIME_STATS()` 和 `portGET_RUN_TIME_COUNTER_VALUE()`。
- 使用 `vTaskGetRunTimeStats()` 输出任务运行时间统计。
- 理解运行时间统计依赖比系统 Tick 更高频的计数源。

## 运行现象
运行后串口输出任务运行时间统计信息，可结合 LED 翻转观察不同任务的运行占比。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


