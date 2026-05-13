# 13 互斥信号量

## 项目简介
本工程使用互斥信号量保护共享资源，用来对比二值信号量示例中的优先级翻转问题。互斥信号量带有优先级继承机制。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `xSemaphoreCreateMutex()` 创建互斥量。
- 使用 `xSemaphoreTake()` / `xSemaphoreGive()` 进入和退出临界资源访问。
- 理解优先级继承如何缓解优先级翻转。
- 对比 `12_模拟优先级翻转` 的运行现象。

## 运行现象
运行后通过串口观察高优先级任务等待互斥量时，持有互斥量的低优先级任务会更快释放资源。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


