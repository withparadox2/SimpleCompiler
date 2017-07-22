照抄javac 1.7，剔除难点繁点。

## stage1
通过各种方式打印出`hello world!`
### 状态：
完成

## stage2
支持基本语法，非常基本，连`while`和`i++`这类都不支持，不支持泛型。
### 状态：
施工中...

## stage3
完善编译流程，支持更多语法特性。

## stage4
支持泛型。

## ...

## stageX
胡作非为。


## run
- install mingw and add /bin to system env
- `mkdir build`
- `cd build` and then `cmake .. -G "MinGW Makefiles"`
- `make` or `mingw32-make`
- `cd ../bin` and then `SimpleCompiler.exe`
- `cd ../TestHelloWorld` and then `java Computer` 