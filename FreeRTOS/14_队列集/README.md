# 14 队列集

## 项目简介
本工程演示队列集。一个队列和一个二值信号量被加入同一个队列集，接收任务统一等待任意成员就绪。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `xQueueCreateSet()` 创建队列集。
- 使用 `xQueueAddToSet()` 将队列和信号量加入队列集。
- 使用 `xQueueSelectFromSet()` 判断哪个成员就绪。
- 根据返回成员分别调用 `xQueueReceive()` 或 `xSemaphoreTake()`。

## 运行现象
按 KEY1 往队列发送键值，按 KEY2 释放二值信号量。Task2 从队列集中识别事件来源并打印结果。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


