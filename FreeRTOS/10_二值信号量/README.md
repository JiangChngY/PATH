# 10 二值信号量

## 项目简介
本工程演示二值信号量的创建、释放和获取。Task1 扫描按键释放信号量，Task2 阻塞等待信号量并在获取后输出提示。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `xSemaphoreCreateBinary()` 创建二值信号量。
- 使用 `xSemaphoreGive()` 释放信号量。
- 使用 `xSemaphoreTake()` 等待和获取信号量。
- 理解二值信号量适合任务同步而不是传递数据。

## 运行现象
按 KEY1 后 Task1 释放信号量，Task2 获取信号量并通过串口打印获取成功。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


