# WiFi+系统信息 QT UI界面

针对一个在rv1106上480*480的屏幕进行设计

实现效果:

1. 屏幕上半部分显示时间与日期

2. 屏幕下半部分分为两块, 左边是一个按键+字符串,显示wifi信息;右边是一个按键+字符串, 显示系统信息与ip地址

3. 按下wifi按键时,切换到wifi配置界面, 其中实现了虚拟键盘, 可以输入密码

## 编译方法

1. 在宿主机编译测试

```bash
mkdir build 
cd build
cmake ..
make
```

2. 在宿主机交叉编译

```bash
mkdir rv_build
cd rv_build
cmake .. -DCMAKE_TOOLCHAIN_FILE=../arm-toolchain.cmake
make
```

## 系统依赖

1. QT5.15.13

2. QT基础组件

后续再检查检查, 可能更新一份dockerfile进来(其实确实没什么依赖)

## Systemd自启动

在顶层目录的 `my_qtScreen.service`中

将其`mv`到`/etc/systemd/system`中, 然后激活它即可.

注意: 需要有这个service才能令reboot调试按键生效,否则就直接退出不重启了

## svg转png

```bash
inkscape wifi.svg --export-type=png \
--export-width=40 --export-height=40 \
-o wifi.png
```

效果为将svg图片转换为40*40大小的png图片