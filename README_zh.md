# device_board_beken

## 简介

本仓用于放置BK7235开发板相关内容，BK7235是博通集成（BEKEN）研发的一款针对IoT应用、高度集成的WiFi6+BLE 5.2 combo SoC，具有资源丰富、性能强大、更高的安全性等特点。

BK7235开发板包含以下几个部分：

>	BK7235核心板，包含匹配电路、滤波电路及晶振电路
>
>	CIS DVP interface
>
>	电源指示灯
>
>	烧录及日志接口
>
>	复位按键

开发板正反面如下：

![开发板正面图](figures/front.png)

![开发板背面图](figures/back.png)

## 目录框架

```
device_board_beken/
├── bk7235x										#BK7235开发板
│   ├── hcs										#HCS配置目录
│   ├── hdf										#HDF配置目录
│   └── liteos_m								#LiteOS-M内核目录
│
├── figures										#图片目录
├── LICENSE										#证书
├── OAT.xml										#OAT文件
├── README_zh.md								#说明文档
└── shields										#Shields配置目录
```

## 开发板特性

芯片框图如下：

![芯片框图](figures/framework.png)

1. BK7235基于WiFi6+BLE5.2双模设计，有更省电、抗干扰能力强、数据传输效率高等特点。
2. BK7235集成了高性能的32-bit RISC-V MCU，主频高达320MHz，集成FPU、MPU并支持DSP指令。EEMBC CoreMark跑分高达3.57 CoreMark/MHz。
3. BK7235片上集成了 4MByte Flash、512KByte SRAM、64KByte ROM、32Byte Efuse以及8Kbit OTP，并且可选4MByte PSRAM的配置。
4. BK7235支持Secure boot及多种硬件加密引擎，并且已经通过了PSA Certified Level 1安全认证。
5. BK7235拥有I2C、SPI、PWM、CAN、IrDA、TOUCH、SARADC、UART、JPEG encoder/decoder 、DVP camera interface等丰富的外设。
6. BK7235支持2.7V~5V宽电压供电，适合家电类应用。
7. BK7235可以工作在-40℃~+125℃的温度范围，适合灯具及户外等应用。

硬件、管脚具体说明可以参考[BK7235技术规格说明书](http://docs.bekencorp.com/spec/BK7235/BK7235%C2%A0Datasheet_V0.1.pdf)。

## OpenHarmony特性

| 组件名       | 能力介绍                                                                                       |
| -------------- | ------------------------------------------------------------------------------------------------ |
| 内核         | LiteOS-M。                                                                                        |
| 上电启动     | 上电启动OpenHarmony。                                                                          |
| WLAN服务     | 提供WLAN服务能力。包括：station和softap模式的连接、断开、状态查询等。 |
| BLE服务      | 提供BLE功能。                                          |
| 外设控制     | 提供操作外设的能力，包括：I2C、SPI、PWM、CAN、IrDA、TOUCH、SARADC、UART、JPEG encoder/decoder 、DVP camera interface等。                      |
| 系统服务管理 | 系统服务管理基于面向服务的架构，提供了OpenHarmony统一化的系统服务开发框架。                  |
| 启动引导     | 提供系统服务的启动入口标识。在系统服务管理启动时，调用boostrap标识的入口函数，并启动系统服务。 |
| 系统属性     | 提供获取与设置系统属性的能力。                                                                   |
| 基础库       | 提供公共基础库能力，包括：KV存储管理等。                                                      |                                                         |
| XTS          | 提供OpenHarmony生态认证测试套件的集合能力。 

## 快速入门

环境搭建、获取源码、编译烧录等内容参考[快速入门](http://docs.bekencorp.com/armino/bk7235/zh_CN/latest/get-started/index.html)。

## 相关仓

[vendor_beken](https://gitee.com/openharmony-sig/vendor_beken)

[device_soc_beken](https://gitee.com/openharmony-sig/device_soc_beken)
