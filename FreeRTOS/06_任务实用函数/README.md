# 06 任务实用函数

## 项目简介
本工程演示 FreeRTOS 常用任务查询和控制 API。Task2 获取 Task1 的优先级，修改优先级后再次查询并输出。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `uxTaskPriorityGet()` 查询任务优先级。
- 使用 `vTaskPrioritySet()` 修改任务优先级。
- 通过任务句柄操作指定任务。
- 理解优先级变化对后续调度的影响。

## 运行现象
串口会先输出 Task1 原始优先级，再输出修改后的优先级；Task1 持续翻转 LED1。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


