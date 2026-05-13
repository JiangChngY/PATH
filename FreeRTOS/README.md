# FreeRTOS 示例工程集合

这个目录保存了一组基于 STM32F103 和 FreeRTOS 的学习示例。每个工程目录都包含独立的 CubeMX 配置、Keil MDK 工程、HAL 外设初始化代码、FreeRTOS 源码/移植文件以及 Core/Src/freertos_demo.c 示例逻辑。

## 通用环境
- MCU: STM32F103Z(C-D-E)Tx，系统时钟 72 MHz
- 工具链: STM32CubeMX + Keil MDK-ARM V5.32
- 串口: USART1, 115200-8-N-1
- 常用外设: LED1/LED2/LED3，KEY1/KEY2/KEY3/KEY4

## 目录说明
顶层目录包含 01 到 20 的独立示例工程，ll 目录下是一套更完整的 01 到 21 汇总示例。每个工程目录内都有自己的 README.md，建议先读对应 README，再打开 Core/Src/freertos_demo.c 对照源码。

## 基本使用
1. 进入某个示例目录。
2. 使用 STM32CubeMX 打开 `HAL_Key.ioc` 查看外设配置。
3. 使用 Keil MDK 打开 `MDK-ARM/HAL_Key.uvprojx` 编译下载。
4. 打开串口助手观察日志，并根据 README 中的说明按 KEY 键验证现象。


