# 05 时间片调度

## 项目简介
本工程演示时间片调度。两个任务执行计数输出，用于观察同优先级任务在 Tick 驱动下轮流运行。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 启用抢占式调度。
- 理解同优先级任务的时间片轮转。
- 观察任务计数输出的交替变化。
- 对比忙等延时和 RTOS 调度行为。

## 运行现象
串口会持续打印 Task1/Task2 的执行次数，可观察两个任务轮流获得 CPU。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


