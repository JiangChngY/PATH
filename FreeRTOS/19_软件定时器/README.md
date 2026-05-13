# 19 软件定时器

## 项目简介
本工程演示 FreeRTOS 软件定时器。创建一个单次定时器和一个周期定时器，通过按键启动或停止，并在回调函数中打印计数。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `xTimerCreate()` 创建软件定时器。
- 理解单次定时器 `pdFALSE` 和周期定时器 `pdTRUE`。
- 使用 `xTimerStart()` 启动定时器。
- 使用 `xTimerStop()` 停止定时器，回调函数中避免阻塞操作。

## 运行现象
按 KEY1 启动两个定时器，串口会看到 timer1 单次回调和 timer2 周期回调。按 KEY2 停止定时器。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


