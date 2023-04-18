# Arduino

## Introduction

Arduino 是一款便捷灵活、方便上手的开源电子原型平台。包含硬件（各种型号的 Arduino 板）和软件（ArduinoIDE）。 引自[Arduino是什么](https://zhuanlan.zhihu.com/p/379811530)

在本次项目中，我们使用Arduino Mega 2560作为主控板，用于控制各个元件，如电机、显示屏。

## 原理图

![Arduino引脚](../../IMG/Arduino.jfif)

## Pin connections

| Pin | Function |
| --- | -------- |
| 2   |          |
| 3   |          |
| 4   |          |
| 5   |          |
| 6   |          |
| 7   |          |
| 8   |          |
| 9   |          |
| 10  |          |
| 11  |          |
| 12  |          |
| 13  |          |
| A0  |          |
| A1  |          |
| A2  |          |
| A3  |          |
| A4  |          |
| A5  |          |
| A6  |          |
| A7  |          |
| A8  |          |
| A9  |          |
| A10 |          |
| A11 |          |
| A12 |          |
| A13 |          |
| A14 |          |
| A15 |          |


## 显示

本次采用LCD1601显示屏，使用I2C通信，使用LiquidCrystal_I2C库

针脚连接如下：

| Pin | Function |
| --- | -------- |
| VCC | 5V       |
| GND | GND      |
| SDA | SDA      |
| SCL | SCL      |

## 电机

本次采用TB6612FNG驱动板

针脚连接如下：

TB6612FNG 1

| Pin | Function |
| --- | -------- |
| VCC | 5V       |
| GND | GND      |
| PWMA | 9      |
| AIN1 | 8      |
| AIN2 | 7      |
| STBY | 6      |
| BIN1 | 5      |
| BIN2 | 4      |
| PWMB | 3      |

TB6612FNG 2

| Pin | Function |    
| --- | -------- |  
| VCC | 5V       |
| GND | GND      |
| PWMA | 9      |
| AIN1 | 8      |
| AIN2 | 7      |
| STBY | 6      |
| BIN1 | 5      |
| BIN2 | 4      |
| PWMB | 3      |