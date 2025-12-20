# 开发环境

1. 编译工具链: `gcc-arm-none-eabi`, 支持`windows`和`linux`
2. 烧录工具: STLink官方烧录工具
3. 调试工具: `OpenOCD` + VSCode插件`Cortex-Debug`

# 构建脚本

`build`目录下`leafcar.sh`即为项目的构建脚本，在使用前先运行`source completion.sh`启用自动补全功能  
`leafcar.sh`使用方法可通过`./leafcar.sh -h`查看

# 下载程序

## OpenOCD

```sh
cd build

# openocd连接目标芯片
openocd -f ../src/openocd.cfg
# 另开一终端连接telnet端口
telnet 127.0.0.1 4444
# 在telnet连接命令行下载程序
halt
# 指定烧写文件以及flash起始地址
flash write_image erase ../src/output/leafcar.bin 0x08000000
reset

# 以上步骤也可用一行命令完成
openocd -f ../src/openocd.cfg -c init -c halt -c "flash write_image erase ../src/output/leafcar.bin 0x08000000" -c reset -c shutdown

# 同理，下载test程序
openocd -f openocd.cfg -c init -c halt -c "flash write_image erase ./output/stm32f103zet6_test.bin 0x08000000" -c reset -c shutdown
```