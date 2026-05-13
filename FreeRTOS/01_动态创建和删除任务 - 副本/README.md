# 01 动态创建和删除任务 - 副本

## 项目简介
本目录是动态创建/删除任务示例的副本工程，主体逻辑与 01 示例一致，可作为对照或备份工程使用。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 通过启动任务统一创建业务任务。
- 在临界区内完成任务创建，避免创建过程被调度打断。
- 使用任务句柄保存 Task1，并在按键触发后删除任务。
- 理解删除任务后句柄置空，避免重复删除。

## 运行现象
运行后 Task1/Task2 周期翻转 LED 并打印信息。按下 KEY1 删除 Task1 后，Task1 不再运行。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


