# 骰子随机生成器

一个基于 Qt 框架开发的骰子随机生成应用程序，演示了 Qt 多线程编程和信号槽机制的使用。

## 功能特点

- 🎲 **骰子随机生成**：随机生成1-6点的骰子
- 🔄 **线程控制**：支持开始、暂停、继续、停止骰子生成
- 📊 **状态显示**：实时显示骰子次数和点数
- 🖼️ **图片展示**：显示对应点数的骰子图片
- 🧵 **多线程处理**：使用子线程处理骰子生成，不阻塞主线程

## 技术栈

- **框架**：Qt 6.x
- **语言**：C++11/14
- **构建工具**：qmake / CMake

## 项目结构

```
Treading_Signals_Slots/
├── main.cpp                 # 应用程序入口
├── mainwindow.cpp           # 主窗口实现
├── mainwindow.h             # 主窗口头文件
├── mainwindow.ui            # 主窗口 UI 设计
├── treading_random.cpp      # 子线程实现
├── treading_random.h        # 子线程头文件
├── resources/               # 资源文件
│   └── dice/                # 骰子图片资源
│       ├── dice_1.png
│       ├── dice_2.png
│       ├── dice_3.png
│       ├── dice_4.png
│       ├── dice_5.png
│       └── dice_6.png
├── dice.qrc                 # 资源文件配置
├── Treading_Signals_Slots.pro    # qmake 项目文件
└── CMakeLists.txt           # CMake 项目文件
```

## 编译和运行

### 使用 qmake

```bash
qmake
make
./Treading_Signals_Slots
```

### 使用 CMake

```bash
mkdir build && cd build
cmake ..
make
./Treading_Signals_Slots
```

## 使用说明

1. **启动线程**：点击「开始线程」按钮启动子线程
2. **开始掷骰子**：点击「开始」按钮开始随机生成骰子
3. **暂停掷骰子**：点击「暂停」按钮暂停骰子生成
4. **停止线程**：点击「停止线程」按钮停止子线程

## 核心技术实现

### 多线程设计

项目使用 Qt 的 `QThread` 类实现多线程：

- `Treading_random` 类继承自 `QThread`，负责在子线程中生成随机骰子
- 通过信号槽机制实现主线程和子线程之间的通信
- 使用 `Start` 和 `Pause` 两个标志位控制线程状态

### 信号槽机制

- **信号**：`qo_dice(QPixmap, int, int)` - 骰子生成信号，包含图片、次数和点数
- **槽函数**：`do_dice(QPixmap, int, int)` - 处理骰子生成结果

### 线程生命周期管理

- 启动线程：调用 `start()` 方法
- 暂停/继续：通过 `Pause` 标志位控制
- 停止线程：设置 `Start` 为 false，等待线程退出

## 许可证

MIT License

## 作者

开发者姓名
