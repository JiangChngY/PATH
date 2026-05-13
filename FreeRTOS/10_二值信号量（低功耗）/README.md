# 10 二值信号量（低功耗）

## 项目简介
本工程在二值信号量同步示例基础上加入 Tickless 低功耗配置。系统空闲时进入低功耗流程，唤醒后继续通过信号量同步任务。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用二值信号量完成按键任务与处理任务之间的同步。
- 启用 `configUSE_TICKLESS_IDLE`。
- 通过 `configPRE_SLEEP_PROCESSING()` / `configPOST_SLEEP_PROCESSING()` 管理睡眠前后外设时钟。
- 理解 Tickless Idle 适合空闲时间较长的应用。

## 运行现象
系统空闲时会进入 Tickless 低功耗处理。按 KEY1 释放信号量，Task2 被唤醒并打印获取成功。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


