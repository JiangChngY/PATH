# 17 任务通知模拟消息队列

## 项目简介
本工程演示使用任务通知传递数值，模拟简单消息队列或消息邮箱。Task1 根据按键发送不同通知值，Task2 等待并读取通知值。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `xTaskNotify()` 写入通知值。
- 使用 `xTaskNotifyWait()` 等待并读取通知值。
- 理解通知值覆盖、位操作、累加等不同动作的差异。
- 适合轻量的一对一消息传递。

## 运行现象
按 KEY1/KEY2 发送不同通知值，Task2 读取后通过串口打印接收到的值。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


