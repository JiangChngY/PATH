# 03 任务挂起与恢复

## 项目简介
本工程演示任务级挂起/恢复以及调度器级挂起/恢复。Task3 读取按键，根据按键操作 Task1 或整个调度器，并通过 `vTaskList()` 打印任务状态。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `vTaskSuspend()` 挂起指定任务。
- 使用 `vTaskResume()` 恢复指定任务。
- 使用 `vTaskSuspendAll()` / `xTaskResumeAll()` 暂停和恢复调度器。
- 打开 `configUSE_TRACE_FACILITY` 与 `configUSE_STATS_FORMATTING_FUNCTIONS` 后使用 `vTaskList()`。

## 运行现象
KEY1 挂起 Task1，KEY2 恢复 Task1，KEY3 挂起调度器，KEY4 恢复调度器。串口会输出任务状态表。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


