# 04 中断管理

## 项目简介
本工程演示 FreeRTOS 中断开关控制以及中断优先级相关实验。Task1 读取按键后执行开关中断操作，工程中还包含 TIM 相关配置用于观察中断行为。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `portDISABLE_INTERRUPTS()` 关闭可屏蔽中断。
- 使用 `portENABLE_INTERRUPTS()` 重新打开中断。
- 理解 FreeRTOS 中断优先级和临界区对任务调度的影响。
- 结合 `Core/Src/tim.c` 与 `stm32f1xx_it.c` 观察定时器中断输出。

## 运行现象
按 KEY1 关闭中断，按 KEY2 打开中断。串口可观察中断输出是否暂停或恢复。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


