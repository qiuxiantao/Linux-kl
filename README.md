# Linux-kl
Linux驱动-内核-内存共享模块
这是一个关于安卓游戏逆向的内核模块
Linux内核模块的编译，这往往需要完整的Linux开发环境
make自动编译，需要下载make
Linux下载指令sudo apt install build-essential

另外还要下载Linux开发的工具包
flex工具
sudo apt-get install flex
flex 是一个用于生成词法分析器的工具，它在编译内核的某些部分时是必需的，特别是当内核配置使用了 kconfig 系统时。

bison工具
sudo apt-get install bison
bison 是一个用于生成语法分析器的工具，它在编译内核的某些部分时是必需的，特别是当内核配置使用了 kconfig 系统时。

libelf-dev开发包
sudo apt-get install libelf-dev
libelf-dev是Linux官方提供的开发扩展包

无论是开发还是编译往往是很困难的
因此我们需要人工智能，进行辅助开发
下面推荐人工智能官网
https://chatgpt.com/---gt4
https://hailuoai.com/--海螺

准备好了需要开发的环境和需求，那我们开始开发来进行编译吧！
我们需要去各大官网下载Linux内核源码
https://github.com/torvalds/linux---完整的Linux
https://www.kernel.org/---Linux内核源码官网
如aosp和小米，需要自己去找
下面开始编译环节
确定你下载文件的目录
使用ls查看当前目录

使用tar解压你下载的文件
tar -xvf 你的文件+后缀名

当然你也可以使用zip
不过需要安装，安装指令sudo apt-get install zip
unzip 你的文件名+后缀名

我这边演示打开
cd linux5.10

接下来开始编译工作
第一先安装内核
1.清理源代码目录
运行以下命令确保源代码目录干净，没有不正确的文件和依赖：
指令make mrproper

2. 配置内核
你可以使用多种工具来配置内核，例如 make menuconfig、make xconfig 或 make gconfig。其中，make menuconfig 是最常用的选项，因为它不依赖于图形库且直观易用：
指令make menuconfig

3编译内核
配置完成后，编译内核：
指令make -j$(nproc）

4.安装模块
编译完成后，安装内核模块：
指令sudo make 你的模块名_内核名
列sudo make modules_install

5.安装内核
最后，安装内核：
sudo make 内核

6.更新引导加载程序
安装新内核后，更新引导加载程序（如GRUB）以确保系统能够识别新内核：
指令sudo update-grub

7. 重启系统
重启系统以应用新内核：
指令sudo reboot

9. 验证安装
重启后，你可以通过以下命令验证新内核是否已成功安装：
指令uname -r


第二开始编译模块
cd Linux—kl
进入你的模块目录
清理
make mrproper
生成配置文件
make menuconfig
输入编译开始指令
make -j$(nproc)
最后会出来文件
xxx.ko
第三安装卸载
加载和卸载模块
在加载或卸载模块之前，请确保你有适当的权限（通常需要root权限）。使用以下命令加载模块：

bash
sudo insmod simple_module.ko
            ----------------内核模块
使用dmesg命令查看内核消息，确认模块是否成功加载：

bash
dmesg | tail
卸载模块使用：

bash
sudo rmmod simple_module
再次使用dmesg命令确认模块是否成功卸载。


