# 12 模拟优先级翻转

## 项目简介
本工程用二值信号量模拟共享资源竞争，观察低优先级任务持有资源、高优先级任务等待资源、中优先级任务抢占 CPU 时产生的优先级翻转现象。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 理解优先级翻转的触发条件。
- 观察低/中/高三个优先级任务之间的运行关系。
- 使用二值信号量保护共享资源时不会自动继承优先级。
- 为后续互斥信号量示例做对比。

## 运行现象
运行后通过串口观察低优先级任务持有资源时，高优先级任务等待而中优先级任务持续运行的现象。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


