# 05 时间片调度

## 项目简介
本工程用于观察 FreeRTOS 多任务调度行为。当前源码创建多个任务并通过 LED 和串口观察任务运行，配置文件中启用了抢占式调度和 `configIDLE_SHOULD_YIELD`。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 关注 `configUSE_PREEMPTION` 与任务优先级对调度的影响。
- 观察多个任务周期延时后进入阻塞态，CPU 如何切换到其他任务。
- 可将两个任务调整为同优先级，用于对比时间片轮转效果。
- 通过串口输出和 LED 翻转判断任务是否被调度。

## 运行现象
Task1/Task2/Task3 周期运行。按 KEY1 会删除 Task1，之后 Task1 输出和 LED1 翻转停止。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


