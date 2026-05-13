# 16 任务通知

## 项目简介
本工程演示任务通知模拟二值信号量。Task1 按键后向 Task2 发送通知，Task2 阻塞等待通知并输出结果。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `xTaskNotifyGive()` 或 `xTaskNotify()` 发送任务通知。
- 使用 `ulTaskNotifyTake()` 接收通知。
- 理解任务通知比二值信号量更轻量，但只能通知指定任务。
- 适合一对一任务同步场景。

## 运行现象
按 KEY1 后 Task1 向 Task2 发送通知，Task2 获取通知后串口打印提示。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


