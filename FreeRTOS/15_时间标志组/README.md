# 15 事件标志组

## 项目简介
本工程演示 FreeRTOS Event Group。Task1 根据按键设置不同事件位，Task2 等待多个事件位满足后执行处理。

## 工程信息
- MCU: STM32F103Z(C-D-E)Tx, 系统时钟 72 MHz
- 工具链: STM32CubeMX `HAL_Key.ioc` + Keil MDK-ARM V5.32 `MDK-ARM/HAL_Key.uvprojx`
- 串口: USART1, 115200-8-N-1, 用于 `printf` 调试输出
- 常用外设: LED1/LED2/LED3, KEY1/KEY2/KEY3/KEY4
- 示例入口: `Core/Src/main.c` 初始化 HAL/GPIO/USART 后调用 `FreeRTOS_Start()`
- 示例逻辑: `Core/Src/freertos_demo.c`, FreeRTOS 配置在 `Core/Inc/FreeRTOSConfig.h`

## 学习重点
- 使用 `xEventGroupCreate()` 创建事件标志组。
- 使用 `xEventGroupSetBits()` 设置事件位。
- 使用 `xEventGroupWaitBits()` 等待多个事件条件。
- 理解事件位是否清除、等待任意位或全部位的区别。

## 运行现象
按 KEY1/KEY2 设置不同事件位。当 Task2 等待的事件组合满足时，串口输出处理信息。

## 使用方法
1. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看或调整外设配置。
2. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx`，编译后下载到 STM32F103 开发板。
3. 打开串口助手，选择 115200 波特率，复位开发板后观察打印信息和 LED/按键现象。


