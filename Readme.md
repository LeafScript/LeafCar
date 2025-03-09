# 开发环境

1. 编译工具链: `gcc-arm-none-eabi`, 支持`windows`和`linux`
2. 烧录工具: STLink官方烧录工具
3. 调试工具: `OpenOCD` + VSCode插件`Cortex-Debug`

# 构建脚本

`build`目录下`leafcar.sh`即为项目的构建脚本，在使用前先运行`source completion.sh`启用自动补全功能  
`leafcar.sh`使用方法可通过`./leafcar.sh -h`查看