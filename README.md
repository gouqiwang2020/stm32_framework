# stm32_framework
```

fork from : https://staok.github.io

   _______  _______  __   __  _______  _______ 
  |       ||       ||  |_|  ||       ||       |
  |  _____||_     _||       ||___    ||____   |
  | |_____   |   |  |       | ___|   | ____|  |	  欢迎来到 stm32_framework
  |_____  |  |   |  |       ||___    || ______| （本项目是“瞰百易”计划的一部分）
   _____| |  |   |  | ||_|| | ___|   || |_____ 
  |_______|  |___|  |_|   |_||_______||_______|
  _______  ______    _______  __   __  _______  _     _  _______  ______    ___   _ 
 |       ||    _ |  |   _   ||  |_|  ||       || | _ | ||       ||    _ |  |   | | |
 |    ___||   | ||  |  |_|  ||       ||    ___|| || || ||   _   ||   | ||  |   |_| |
 |   |___ |   |_||_ |       ||       ||   |___ |       ||  | |  ||   |_||_ |      _|
 |    ___||    __  ||       ||       ||    ___||       ||  |_|  ||    __  ||     |_ 
 |   |    |   |  | ||   _   || ||_|| ||   |___ |   _   ||       ||   |  | ||    _  |
 |___|    |___|  |_||__| |__||_|   |_||_______||__| |__||_______||___|  |_||___| |_|
```



https://staok.github.io:一个志在实现STM32F1、F2和F4工程模板的项目，包含HAL库的高级封装、菜单库、有限状态机模板，集成了FreeRTOS、LWIP、FATFS、DSP、USB、IAP等等的组件，以及未来将加入的加密、BPNN、最小二乘、音频图片视频解码、LittlevGL等诸多常用的算法或组件，并具有良好的易用性、解耦性和可剪裁性！

那么就有小伙伴说了：“STM32这么简单，有啥玩的”，那么您玩出一个花儿来给我看看，Talk is cheap。

本项目的说明介绍等等部分遵循“二项玻”的第二则进行。

除了开源库之外的代码，每一行都经过手动移植、修改、检查和运行，是有灵魂的。

喜欢的话，点个小star鸭~

仓库容量比较大（软件工程 约 200MB，资料 约 400MB）。

------

## 近期计划

这段时间比较忙学业，日常想法都放在了 “模板要添加的内容.txt” 里面。

理解了原理的东西，通用性低的东西，就不用时间去走一遍前人走过的路了，就酱

2021.6 最新留言：作者对目前的代码架构很不满意，已经打算在未知的未来进行 一部分大改和一部分直接重构，所以驱动写法可以看一看和复制粘贴，但是不推荐直接用工程本体，将来一定再改善到非常好。**所以本项目暂时烂尾，见谅。**

## 各个进展

下面三个看一看驱动就好

### STM32F1xx

~~不是目前主要完善的对象，已经完成的大部分外设驱动并且调好~~

### STM32F207IE

~~不是目前主要完善的对象，已经粗糙的完成的大部分外设驱动，有很多bugs~~

### STM32F4xx

~~目前主要完善的对象，已经完成大部分外设驱动~~

## 计划外设和组件

（加**粗体**为暂时尚未实现的外设或者组件）

-   **不会支持的外设**：LTDC LCD \ IRDA \ CRYP \ HASH

- **暂时没有支持的组件：**（暂时无限延期）

  线性回归算法库

  常用校验、加密算法（3DES、AES、RC6、TEA、RSA、MD5、SHA1、SHA256）

  Modbus 协议

  DCMI 和 IO 驱动 OV7725、OV2640 和 OV5650 摄像头

  音频编解码（MP3转PCM，外接一个功放直接播放）

  MQTT 协议（包含 cjson，LWIP 应用）

  JPEG、GIF 解码和BMP编解码

  LittlevGL

  NN（CMSIS 包中关于 NN 的内容，吸收进来）

  [IAP](https://github.com/Staok/u-iap)（自实现）

-   已经支持的外设：

    大部分HAL库外设驱动的高级封装（ MCO \ RTC \ CRC \ TIM \ ADC \ DAC \ IWDG \ USART \ SPI \ WFI \ FLASH \ **IAP** \ IO \ SDIO \ **DCMI** \ FSMC \ DMA \ RNG \ DSP \ FPU \ USB \ CAN \ Ethernet）

-   已经支持的组件：

    [FreeRTOS](https://www.freertos.org/) 10.3.1

    [LWIP](http://savannah.nongnu.org/projects/lwip/) 2.1.2（支持UDP、TCP Server和TCP Client 的RAW API，HTTP WebServer）

    [FATFS](http://elm-chan.org/fsw/ff/00index_e.html) ff14

    [USB （ST UM1021）](https://www.st.com/en/embedded-software/stsw-stm32046.html)

    [ARM CMSIS](https://github.com/ARM-software/CMSIS_5) 5.7.0 DSP Lib

    fsm 有限状态机库（自实现）

    menu 菜单框架（自实现）

    内存管理（malloc 和 free）（自实现）

    软件模拟 I2C \ SPI 库（自实现）

    [PID 库](https://github.com/Staok/pid_realize)（自实现）
    
    工程添加下载到 RAM 执行的功能
    
    [无依赖的 sprintf 库](https://github.com/mpaland/printf)
    
    [lwrb (软件 ringbuf，FIFO 缓冲)](https://github.com/MaJerle/lwrb)

## 组件和外设说明

（标为“缺省”的为暂未实现的，以后会加上- -，工作量好大的！ ）

-   **所有基础外设均在sys_config.h（F1）里或者PeriphConfigCore.h（F2和F4）里通过宏定义配置，每一个宏定义旁边均有使用说明的注释**，有详细注释，不看不会用
-   外设驱动代码分别在sys_config.c和periphconfig.c两个文件里，其所有的外设API声明均在sys.h里，所有的独立GPIO的API均在periphconfig.h里，用户程序调用外设控制API时，按需调用这两个文件即可
-   MCU外设配置的统一函数为void sys_MCU_Init_Seq(void)，一般无需改动；外接器件初始化统一函数为void sys_Device_Init_Seq(void)，需要自行按需修改
-   本框架是正经的框架，有命名规范和应用规范的说明，在sys.h（F1）里或者PeriphConfigCore.h（F2和F4）里的上面
-   **F4模板请看Docs手册文档，里面包含了所有已经支持的组件和外设的使用说明**

### 是否启用OS

-   sys.h和FreeRTOSConfig.h里面的宏定义SYSTEM_SUPPORT_OS：是否开启FreeRTOS，不开为裸跑；具体应用逻辑编写和任务配置分别在TaskConfig.c和BareConfig.c里面

    FreeRTOS说明：所有API均罗列在TaskConfig.h里！关于FreeRTOS的信号量和事件标志组，均用任务通知替代！关于FreeRTOS的任务与任务、任务与中断间的通讯只用消息队列，功能最全！

### 组件

-   Menu 菜单框架：SYSTEM_SUPPORT_Menu：提供一个作者自己实现的实现菜单功能的模板；推荐作为标准常用，应用逻辑围绕这个菜单框架展开编写；具体文件在sys_menu.c里面；按照里面的注释，void keyProcess(void)函数负责扫描输入，void menuProcess(void)函数负责执行菜单功能，这两个函数必须放到一个循环里一直执行

-   fsm 有限状态机：提供一个作者自己实现的有限状态机（fsm），用于管理逻辑较为复杂的程序，帮助更好的写阅读性强的状态图实现，详细说明和具体用法在fsm.c和fsm.h文件内（推荐：sys_menu 用于管理输入（按键等）、输出（屏幕等）和系统参数的用户控制，fsm 用于管理 用状态图 描述的多状态、多条件的复杂逻辑任务）

-   string、sprintf库：SYSTEM_SUPPORT_MyString：提供一个实现了string.h大部分字符操作函数的库；具体作用看MyString.c文件里的注释

-   string、sprintf库：SYSTEM_SUPPORT_sprintf：提供一个无依赖的独立实现的sprint库，github开源库from：mpaland/printf；经过修改，可以实现对指定串口发送printf格式化的字符串

-   PID算法库：SYSTEM_SUPPORT_pid：提供一个作者自己实现的pid算法实现库，集成了积分分离和变限积分，以及可选的不完全微分和微分先行，具体用法看pid.h里面

-   软件ringbuf，FIFO缓冲：由于应对大数据传输时，一个开源FIFO库

-   内存管理（malloc和free）：提供一个自实现的内存分配和释放函数，可用于内部RAM和外部RAM，参考了正点原子的“内存管理”章节的源代码

-   LittlevGL：暂时缺省

-   LWIP 2.1.2：（一个人精力有限，目前只有F4的模板比较完善，请注意！）在lwipopt.h里面进行配置，支持UDP、TCP Server、TCP Client，HTTP Server，

    详细使用说明请看“Docs.bin”里面的LWIP章节

-   FATFS ff14：SYSTEM_FATFS_ENABLE：已经支持SDIO SD卡，SPI SD卡，SPI FLASH和USB HOST MSC，详细使用说明请看“Docs.bin”里面的FATFS章节

-   USB （ST UM1021）：详细使用说明请看“Docs.bin”里面的“STM32 USB库 使用”章节，里面介绍了USB Device的VCP、MSC和HID以及USB HOST的MSC和HID

-   IAP：[详见这里](https://github.com/Staok/u-iap)

### 外设

（再次说明，具体使用方法看相应宏定义旁的注释，均在sys_config.h（F1）里或者PeriphConfigCore.h（F2和F4）里）

-   MCO：SYSTEM_MCO_PA8_OUT：设置PA8为MCO输出，默认时钟源为HSE
-   RTC：SYSTEM_RTC_ENABLE：配置使用RTC
-   CRC：SYSTEM_CRC_ENABLE：配置使用CRC循环冗余校验，外设初始化时进行一次硬件CRC32测试
-   RNG：SYSTEM_RNG_ENABLE：硬件32位真随机数生成器
-   TIM4：STSTEM_TIM4_ENABLE：模板基本功能，必须开启！使用通用定时器4，提供时基，默认10ms周期中断
-   TIM3：STSTEM_TIM3PWM_ENABLE：模板基本功能，必须开启！使用通用定时器3，默认不开启中断，提供四路PWM输出；引脚重映射、中断、频率、占空比均提供修改
-   TIM2：STSTEM_TIM2_ENABLE：通用定时器2，功能自定，初始化函数在PeriphCconfig.c里面定义；提供定时器中断、PWM、输入捕获和正交解码功能，用户按需自定
-   TIM1\8：STSTEM_TIM1/8_ENABLE：默认专门产生PWM，提供三路带死区（默认3us，可调）的互补PWM输出和一路PWM输出，引脚映射、频率、刹车、极性等均可按需调节
-   TIM6/7：STSTEM_TIM6/7_ENABLE仅用于实现定时器中断，提供一些同步和时基的工作，减轻TIM4的压力
-   ADC1：SYSTEM_ADC1_ENABLE：启否ADC1；提供添加规则组通道、是否连续扫描、是否使用DMA等可选功能；有详细注释
-   IWDG：SYSTEM_IWDG_ENABLE：开启独立看门狗，默认1S的喂狗周期，默认在TIM4定时中断里喂狗，用IWDG必开TIM4
-   USART：SYSTEM_UARTx_ENABLE：开启串口，x = 1、2和3；引脚重映射和波特率可选
-   SPI：SYSTEM_SPIx_ENABLE：开启SPI，x = 1和2；尽量只用其中一个，多个器件用多个SS使能端，不提供引脚重映射
-   WFI：SYSTEM_StdbyWKUP_ENABLE：使用待机-低功耗模式
-   FLASH：SYSTEM_FLASH_IAP_ENABLE：启用对内部FLASH储存空间编程
-   IO：GPIO相关的初始化、输入出和位带操作以及改变入出模式的调用口均在PeriphCconfig.c和PeriphCconfig.h里，有详细注释
-   DAC：SYSTEM_DAC_OUTx_ENABLE：大容量芯片系列外设，x=1~2
-   SDIO：SYSTEM_SDIO_SD_ENABLE：大容量芯片系列外设，底层API已经写入FATFS的底层，请用FATFS的API以文件方式操作
-   DMA：默认用于ADC1的多通道扫描模式DMA传送（如果开启SYSTEM_ADC1_useScan的话），另提供驱动代码的模板以供参考
-   FSMC：SYSTEM_FSMC_ENABLE：大容量芯片系列外设，可以用于外部SRAM或者LCD驱动，默认外部RAM用FSMC的块1区3，LCD用FSMC的块1区4，慎改动；就不支持各种FLASH了，MCU毕竟程序写不大，就算大了就用SPI的FLASH，也节约IO口。LCD的相关API说明在TFTLCD.h里面，字库也可剪裁
-   DSP\FPU：在F4模板中默认添加，可用直接调用相关API，详情看Docs手册文档的DSP专题
-   CAN：SYSTEM_CAN1_ENABLE：提供设置接收时只关心的帧类型和ID，提供收发API，收发可用设置帧类型和ID，STM32的CAN目前只有数据帧和遥控帧两种，每种都可以设置是标准标识符还是扩展标识符，CAN有关的学习记录和手册在Tips文件夹内，不熟悉的可用阅读，人话挺多的
-   Ethernet：看“LWIP组件”
-   DCMI：暂时没有计划去支持
-   USB：详细使用说明请看“Docs.bin”里面的“STM32 USB库 使用”章节，里面介绍了USB Device的VCP、MSC和HID以及USB HOST的MSC和HID

如果觉得好用，使用时还请别忘加上本仓库的地址哦：https://github.com/Staok/stm32_framework

```
Author : Staok
Email : superxhy@qq.com
Repo : https://github.com/Staok/stm32_framework
System is starting...
...
System init over!
```

## STM32 学习路线

[超快 STM32 系统入门指南 - 知乎 (zhihu.com)](https://zhuanlan.zhihu.com/p/406496862)。

## STM32 资源站

各大厂家对 STM32 出了特别特别特别丰富的教程和例程，如果比较熟悉外设，直接看他们的例程源码即可，快速掌握各种技巧。各大家的官网和资料下载地址如下：

正点原子：

- [OpenEdv-开源电子网-正点原子论坛](https://link.zhihu.com/?target=http%3A//www.openedv.com/forum.php) 有问题可以在这里找，也可以平时没事逛一逛。
- [正点原子官方的个人空间_ 哔哩哔哩 _bilibili](https://link.zhihu.com/?target=https%3A//space.bilibili.com/394620890) 所有教学视频。
- [原子哥教学平台，专注电子技术教学 (yuanzige.com)](https://link.zhihu.com/?target=https%3A//www.yuanzige.com/)。
- [正点原子资料下载中心 — 正点原子资料下载中心 1.0.0 文档 (openedv.com)](https://link.zhihu.com/?target=http%3A//www.openedv.com/docs/index.html) 各种开发板的电路原理图、例程和教程 统统薅走！

野火：

- [野火电子论坛 - (firebbs.cn)](https://link.zhihu.com/?target=https%3A//www.firebbs.cn/) 论坛。
- [野火_ firege的个人空间 _ 哔哩哔哩 _bilibili](https://link.zhihu.com/?target=https%3A//space.bilibili.com/356820657) 所有教学视频。
- [野火产品资料下载中心 — 野火产品资料下载中心 文档 (embedfire.com)](https://link.zhihu.com/?target=http%3A//doc.embedfire.com/products/link/zh/latest/index.html%23) 薅羊毛。

安富莱：

- [硬汉嵌入式论坛 - Powered by Discuz! (armbbs.cn)](https://link.zhihu.com/?target=http%3A//www.armbbs.cn/) 论坛。
- [武汉安富莱电子有限公司官方网站。STM32开发板,无线IO模块,H7-TOOL工具,嵌入式系统 (armfly.com)](https://link.zhihu.com/?target=http%3A//www.armfly.com/) 官网。
- [【安富莱】各种开发板和模块的资料下载汇总贴（2021-08-30） - 安富莱电子 - 博客园 (cnblogs.com)](https://link.zhihu.com/?target=https%3A//www.cnblogs.com/armfly/p/11270280.html) 资料下载站。

硬石：

- [硬石电子 - 社区 (ing10bbs.com)](https://link.zhihu.com/?target=http%3A//www.ing10bbs.com/forum.php) 论坛。
- [硬石科技的个人空间_ 哔哩哔哩 _bilibili](https://link.zhihu.com/?target=https%3A//space.bilibili.com/324575605) 教学视频。
- [硬石开源资料下载链接(该帖总是最新资料) - 硬石资料更新 - 硬石社区社区 (ing10bbs.com)](https://link.zhihu.com/?target=http%3A//www.ing10bbs.com/forum.php%3Fmod%3Dviewthread%26tid%3D1458%26extra%3Dpage%3D1) 资料下载站。

原子、野火、安富莱和硬石各有侧重，可以广泛借鉴：野火、正点原子 非常全面；安富莱偏 M4F 的 DSP 使用；硬石偏电机控制，有 步进电机、有刷电机、无刷电机。

STM32 官网资料下载站，是源头，可以找到任何官方的东西，包括选型手册，好好把握：

- 中文站：[STM32 | 产品 | STM32/STM8 | MCU单片机 | 意法半导体STM | 意法半导体STM | STM32/STM8微控制器 | MCU单片机 (stmcu.com.cn)](https://link.zhihu.com/?target=https%3A//www.stmcu.com.cn/Product/pro_detail/cat_code/PRODUCTSTM32/family/81/layout/product)。
- ST 官方：[微控制器与微处理器 - STMicroelectronics](https://link.zhihu.com/?target=https%3A//www.st.com/zh/microcontrollers-microprocessors.html)。

