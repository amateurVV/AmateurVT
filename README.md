# VT 基础
* 测试CPU : 11th Gen Intel(R) Core(TM) i7-1185G7 @ 3.00GHz
* 测试系统 : Win10 x64 22H2
* 虚拟机环境:VMware 17 Pro

# 注意事项
* 因为使用者的CPU特殊功能参差不齐,如果要兼容更多CPU,必须对的所有CPU的特殊功能进行询问
* 此例没有考虑更多的独有CPU特性,请在调试模式运行,对使用者CPU的特性做相应的修改,需要对CPU的相关知识有所了解

# Bug
* 在WinDbg PreView调试模式下,会出现 Unknown Exception C0000096!
* Cr0 写入,GDTR 写入,之类的特权指令引起的未知异常
* 不在调试模式下,则一切正常.
* DebugView 打印时常随机引发宕机
