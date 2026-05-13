# 08 绝对延时与相对延时

## 项目简介
本工程用于对比相对延时和绝对延时。Task1 使用普通延时，Task2 目标是使用固定周期延时来减少循环执行时间造成的周期漂移。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `vTaskDelay()` 实现相对延时。
- 使用 `vTaskDelayUntil()` 实现绝对周期延时。
- 理解 `xTaskGetTickCount()` 和上次唤醒时间的作用。
- 对比两个 LED 的周期稳定性。

## 运行现象
Task1/Task2 分别翻转 LED。观察相对延时和绝对延时在周期稳定性上的差异；若修改源码，请确保绝对延时函数名为 `vTaskDelayUntil()`。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


