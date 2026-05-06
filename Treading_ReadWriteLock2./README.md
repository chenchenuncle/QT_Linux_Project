# Treading_ReadWriteLock 项目介绍

<img width="1917" height="1077" alt="截图 2026-05-06 18-17-10" src="https://github.com/user-attachments/assets/edd61dc6-f713-4166-8843-043524b2be5c" />

## 项目概述

<img width="1917" height="1077" alt="截图 2026-05-06 18-18-30" src="https://github.com/user-attachments/assets/674902a9-a1d0-42c5-a07d-eac36ff592c3" />
<img width="1917" height="1077" alt="截图 2026-05-06 18-20-45" src="https://github.com/user-attachments/assets/1cfdc720-620e-4131-90b1-59589a2c5bd9" />

本项目是一个基于 Qt 框架的多线程骰子投掷应用程序，演示了 Qt 中的读写作战（ReadWriteLock）和条件变量（WaitCondition）的使用方法。

## 功能特性

### 核心功能
- **骰子投掷**：通过 `Treading_random` 线程生成 1-6 的随机骰子点数
- **数据读取**：两个独立的读取线程分别读取骰子数值和对应的图片资源
- **线程同步**：使用 `QReadWriteLock` 实现多线程安全的数据访问
- **信号槽通信**：主线程与子线程之间通过信号槽机制进行数据传递

### 界面展示
- 显示每次投掷的骰子点数（第 N 次抛骰子，点数为 X）
- 实时显示骰子图片

## 架构设计

### 线程结构

| 线程类 | 职责 | 锁定方式 |
|--------|------|----------|
| `Treading_random` | 生成随机骰子点数 | 写锁 `lockForWrite()` |
| `Treading_read_int` | 读取骰子数值并发送信号 | 读锁 `tryLockForRead()` |
| `Treading_read_str` | 读取骰子图片路径并发送信号 | 读锁 `tryLockForRead()` |

### 同步机制

```cpp
QReadWriteLock lock;      // 读写锁，保护共享数据
QWaitCondition condition; // 条件变量，协调线程执行
```

- **写线程**（`Treading_random`）：生成随机数后解锁并唤醒所有等待线程
- **读线程**（`Treading_read_int/str`）：等待条件变量，收到信号后读取数据

### 类设计

#### Treading_random 类
- 继承自 `QThread`
- 重写 `run()` 方法
- 使用 `QRandomGenerator64` 生成 1-6 的随机整数

#### Treading_read_int 类
- 继承自 `QThread`
- 信号：`qo_read(int sum, int dice)` - 发送骰子次数和点数
- 重写 `run()` 方法从共享数据中读取数据

#### Treading_read_str 类
- 继承自 `QThread`
- 信号：`qo_read(QString str)` - 发送骰子图片路径
- 根据骰子点数生成对应图片资源路径

### MainWindow 类
- 主窗口类，继承自 `QMainWindow`
- 管理三个子线程的启动和停止
- 通过信号槽接收子线程发送的数据并更新界面

## 文件结构

```
Treading_ReadWriteLock/
├── main.cpp                 # 应用程序入口
├── mainwindow.h             # 主窗口类头文件
├── mainwindow.cpp           # 主窗口类实现
├── mainwindow.ui            # Qt Designer 界面文件
├── treading_random.h        # 多线程类头文件
├── treading_random.cpp      # 多线程类实现
├── treading_random.ui       # （未使用）
├── Treading_ReadWriteLock.pro  # Qt 项目文件
├── dice.qrc                 # Qt 资源文件，包含骰子图片
└── build/                   # 构建目录
```

## 技术要点

### QReadWriteLock 用法
- `lockForWrite()`：写锁，排他访问
- `tryLockForRead()`：尝试获取读锁，非阻塞
- 支持多个线程同时获取读锁，提高并发性能

### QWaitCondition 用法
- `wait(QReadWriteLock*)`：释放锁并等待唤醒
- `wakeAll()`：唤醒所有等待的线程
- 必须与锁配合使用

### QRandomGenerator64 用法
- `QRandomGenerator64::global()`：获取全局随机数生成器
- `bounded(1, 7)`：生成 [1, 7) 范围内的随机整数（1-6）

## 使用说明

### 编译运行
```bash
# 使用 qmake 构建
qmake Treading_ReadWriteLock.pro
make

# 或使用 Qt Creator 打开 .pro 文件进行构建和运行
```

### 操作流程
1. 点击"启动"按钮启动三个子线程
2. 骰子线程每秒生成一个新的随机骰子点数
3. 两个读取线程每秒读取并显示骰子信息
4. 点击"停止"按钮终止所有子线程

## 注意事项

1. **线程安全**：所有对共享变量 `sum` 和 `dice` 的访问都通过读写锁保护
2. **资源管理**：主窗口析构时会等待所有子线程结束
3. **信号槽连接**：使用 Qt::AutoConnection 自动选择连接类型

## Qt 知识学习要点

本项目涵盖了以下 Qt 核心概念：

1. **QThread 多线程编程**：线程的创建、启动和终止
2. **QReadWriteLock 读写锁**：多读者单写者的同步机制
3. **QWaitCondition 条件变量**：线程间事件通知
4. **信号与槽机制**：跨线程数据传递
5. **QRandomGenerator64**：Qt 6 的随机数生成
6. **QThread::terminate() 与 QThread::wait()**：线程终止与等待
