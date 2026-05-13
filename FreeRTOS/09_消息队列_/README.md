# 09 消息队列

## 项目简介
本工程演示 FreeRTOS 队列的创建、发送和接收。示例包含一个保存按键值的小数据队列，以及一个用于传递大数据地址的队列。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `xQueueCreate()` 创建队列。
- 使用 `xQueueSend()` 从任务向队列发送数据。
- 使用 `xQueueReceive()` 阻塞等待队列数据。
- 理解小数据拷贝入队和大数据指针入队的区别。

## 运行现象
按 KEY1 或 KEY2 将键值写入 `queue1`，Task2 从队列读取并打印。按 KEY3 触发大数据队列示例，Task3 读取后打印数据内容。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


