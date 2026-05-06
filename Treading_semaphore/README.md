# 双缓冲区生产者-消费者示例程序

## 程序简介

这是一个基于Qt框架的多线程演示程序，实现了双缓冲区的生产者-消费者模型。程序使用信号量（QSemaphore）和互斥锁（QMutex）来协调两个工作线程之间的数据交互。

## 功能说明

- **生产者线程（thread_read）**：负责向缓冲区写入数据
- **消费者线程（thread_close）**：负责清空已读取的缓冲区
- **信号槽通信**：通过自定义信号将缓冲区数据传递到主界面显示

## 编译运行

### 环境要求
- Qt 6.10.2 或更高版本
- 支持C++17的编译器

### 编译步骤
```bash
cd build/Desktop_Qt_6_10_2-Debug
make
./Treading_semaphore
```

## 运行界面

### 初始状态

<img width="1917" height="1077" alt="图片" src="https://github.com/user-attachments/assets/d3057973-b722-43a2-8381-281880ed2e03" />


### 启动后的运行界面

<img width="1917" height="1077" alt="图片" src="https://github.com/user-attachments/assets/44e0bcfd-bba2-4562-a557-bbda24f17fd9" />


### 缓冲区数据显示

<img width="1917" height="1077" alt="图片" src="https://github.com/user-attachments/assets/1aca4c50-954d-4790-ab5e-d67ff1899c45" />


## 核心代码结构

### 线程类
- `thread_read`：生产者线程类
- `thread_close`：消费者线程类

### 共享资源
- `buff1[10]`：缓冲区1
- `buff2[10]`：缓冲区2
- `semaphore`：信号量，控制缓冲区访问
- `mutex`：互斥锁，保护共享变量

## 操作说明

1. 点击"启动"菜单开始线程运行
2. 观察界面中显示的缓冲区数据
3. 点击"停止"菜单终止线程

## 技术要点

- QThread多线程编程
- QSemaphore信号量同步
- QMutex互斥锁保护共享数据
- 信号槽跨线程通信

## 注意事项

- 确保Qt开发环境正确配置
- 运行时注意观察缓冲区数据的交替变化
