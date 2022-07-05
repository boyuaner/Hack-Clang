## Plugin usage:

```c++
clang  ~/llvm-project/build/mytest.c -fsyntax-only  -Xclang -load -Xclang ~/llvm-project/build/lib/MuteUndefined.so -Xclang -add-plugin -Xclang MuteUndefined
```

## 环境说明

1:测试环境:Ubuntu 18.04.1

2:clang环境:
clang version 10.0.0 
Target: x86_64-unknown-linux-gnu
Thread model: posix

3:文件对应编译失败原因列表

- [x] 1.cpp       缺少头文件 
  插入AST节点声明
- [ ]   2.cpp       缺少宏定义申明 （使用了未定义的宏）
- [ ]   3.cpp       缺少函数申明 （使用了未定义函数）
- [ ]   4.cpp       缺少全局变量申明 （使用了未定义全局变量）
  根据定义修改声明类型/删除声明节点
- [ ]   5.cpp       函数定义类型与申明类型不匹配
- [x]   // 6.cpp       函数缺少return语句(编译可以通过)
  未知类型替换成整型/强转
- [ ]   7.cpp       函数调用参数类型不匹配
- [ ]   8.cpp       不能识别的关键字（类型）
- [ ]   9.cpp       不同系统配置文件（比如说windows 文件在Linux 环境编译之类的）

Solution
1. 使用PPCallback接入preprocessor in clang plugin
2.3.4. 取消未定义变量报错 -Wl,-undefined,dynamic_lookup 问题：局部变量未声明也会被忽略 == 可怎么确定未定义的是什么类型？

CloneFunction
https://stackoverflow.com/questions/40141824/clonefunction-then-replace-original-function-with-cloned-one-in-llvm-opt-pass

1) Create a new function with the desired function type.
2) Clone the body of the old function into the new function
3) Find all calls calling the old function and replace them with calls to the new function.
5. 按照定义类型重写函数
6. 在缺少return报错的hooks中将该函数改为无返回值函数 or FunctionDecl.setHasImplicitReturnZero(true);
7. 强。。强转？ FunctionDecl.getParamDecl

8.9.目前没思路



已经完结，G