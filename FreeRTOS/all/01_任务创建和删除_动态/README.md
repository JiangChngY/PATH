# 01 任务创建和删除 - 动态

## 项目简介
本工程是 all 集合中的动态任务创建与删除示例。启动任务动态创建业务任务，KEY1 触发删除 Task1。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- `xTaskCreate()` 动态创建任务。
- `vTaskDelete()` 删除任务。
- 任务句柄用于后续控制指定任务。
- LED 和串口用于观察任务是否继续运行。

## 运行现象
Task1/Task2 周期翻转 LED。按 KEY1 删除 Task1 后，LED1 翻转和 Task1 输出停止。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


