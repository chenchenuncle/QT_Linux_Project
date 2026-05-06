# 骰子随机生成器（Dice Random Generator）

一个基于 Qt 框架的多线程骰子生成应用，演示 Qt 的多线程与信号槽机制。

## 功能特点

- 🎲 随机生成1-6点的骰子（展示图片与点数）
- 🔄 控制骰子生成线程（开始、暂停、继续、停止）
- 📊 实时显示骰子次数和点数
- 🧵 使用子线程，主界面无卡顿

## 技术栈

- 框架：Qt 6.x
- 语言：C++11/14
- 构建：qmake / CMake

## 项目结构

（此处注意目录一致性，把目录名、文件名与实际代码结构保持一致）

```
Threading_Dice_Generator/
├── main.cpp
├── mainwindow.cpp/.h/.ui
├── threading_random.cpp/.h
├── resources/dice/*.png
├── dice.qrc
├── Threading_Dice_Generator.pro
└── CMakeLists.txt
```

## 编译与运行

> 推荐 Qt Creator 直接打开 .pro 工程。或命令行：

**qmake:**
```bash
qmake
make
./Threading_Dice_Generator
```

**CMake:**
```bash
mkdir build && cd build
cmake ..
make
./Threading_Dice_Generator
```

## 使用方法

1. 点击「开始线程」启动生成骰子线程
2. 点击「开始」开始掷骰子
3. 点击「暂停」暂停生成
4. 点击「停止线程」终止生成

## 核心实现

- **多线程**：QThread 子类专门生成随机骰子
- **信号槽**：线程结果通过 `qo_dice(QPixmap, int, int)` 信号发送，主界面用槽函数接收
- **线程控制**：Start、Pause 标志位控制生命周期

## License

[MIT](LICENSE)

## 作者

陈才坚

MIT License

## 作者

陈才坚
