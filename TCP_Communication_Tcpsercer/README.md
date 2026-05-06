# TCP_TcpServer 2.0

基于 Qt 框架开发的 TCP 服务器应用程序，支持多客户端连接管理、消息转发和实时状态监控。

## 功能特性

### 核心功能
- **多客户端连接管理**：支持同时连接多个客户端，通过唯一 UUID 标识每个连接
- **客户端间消息转发**：服务器作为中转站，实现客户端之间的消息传递
- **实时状态监控**：实时显示连接状态、客户端 IP 地址和端口信息
- **连接管理**：支持开始监听、停止监听、断开指定客户端连接

### 界面功能
- 工具栏快捷操作（开始监听、停止监听、清空文本框、退出）
- 客户端 ID 选择下拉框
- 消息输入和发送功能
- 消息显示区域（支持富文本显示）
- 状态栏显示当前监听状态和临时 ID

## 界面预览

### 服务器启动界面
<img width="1917" height="1077" alt="图片" src="https://github.com/user-attachments/assets/2a852dcf-bc69-4dd7-8df2-735ca4aefff3" />

### 监听状态界面
<img width="1917" height="1077" alt="图片" src="https://github.com/user-attachments/assets/a71140ed-3b98-4a92-9aae-0c1faa33eb26" />

### 客户端连接成功
<img width="1917" height="1077" alt="图片" src="https://github.com/user-attachments/assets/8214d437-c84b-4583-a617-1700b92308c6" />

### 消息收发界面
<img width="1917" height="1077" alt="图片" src="https://github.com/user-attachments/assets/e4e7949b-2657-415a-8463-753d4f280672" />

## 技术实现

### 核心技术栈
- **框架**：Qt 6.10.2
- **语言**：C++11
- **网络模块**：Qt Network（QTcpServer、QTcpSocket）

### 架构设计

#### 客户端管理机制
- 使用 `QHash<QUuid, QTcpSocket*>` 存储客户端连接映射
- 每个客户端连接时自动分配唯一 UUID
- 客户端断开时自动清理资源

#### 消息传输协议
- 消息格式：`[目标UUID][消息内容]`
- 服务器解析目标 UUID 后转发消息
- 支持中文消息传输

#### 信号槽机制
```cpp
// 客户端连接相关
connect(tcp_ID, &QTcpSocket::readyRead, this, &MainWindow::do_readyRead);
connect(tcp_ID, &QTcpSocket::disconnected, this, &MainWindow::do_disconnected);

// 服务器监听相关
connect(tcpserver, &QTcpServer::newConnection, this, &MainWindow::do_newConnection);
connect(tcpserver, &QTcpServer::pendingConnectionAvailable, this, &MainWindow::do_pendingConnectionAvailable);
```

### 关键类与方法

| 类/方法 | 功能说明 |
|---------|----------|
| `MainWindow` | 主窗口类，承载所有业务逻辑 |
| `do_newConnection()` | 处理新客户端连接事件 |
| `do_readyRead()` | 接收并处理客户端消息 |
| `do_disconnected()` | 客户端断开连接时清理资源 |
| `do_pendingConnectionAvailable()` | 处理待处理连接 |
| `Handle(QByteArray&)` | 解析消息中的目标 UUID |
| `Client_ID(QUuid, QByteArray&)` | 向指定客户端发送消息 |
| `Error(QTcpSocket*, QString)` | 向客户端发送错误信息 |

## 使用说明

### 启动服务器
1. 点击「开始监听」按钮启动服务器
2. 在状态栏查看监听端口和临时 ID
3. 客户端可通过服务器 IP 和端口进行连接

### 发送消息
1. 在下拉框选择目标客户端 ID
2. 在输入框输入消息内容
3. 点击「发送消息」按钮发送

### 停止监听
- 点击「停止监听」按钮停止服务器
- 所有客户端连接将被断开

### 清空日志
- 点击「清空文本框」按钮清除消息显示区域内容

## 项目结构

```
TCP_Tcpsercer2.0/
├── main.cpp                    # 程序入口
├── mainwindow.h                # 主窗口类声明
├── mainwindow.cpp              # 主窗口类实现
├── TCP_TcpServer.pro           # Qt 项目文件
├── .qtcreator/                 # Qt Creator 配置目录
│   └── TCP_TcpServer.pro.user  # 用户配置
└── build/                      # 编译输出目录
    └── Desktop_Qt_6_10_2-Debug/ # Debug 构建目录
```

## 编译与运行

### 编译环境要求
- Qt 6.10.2 或更高版本
- CMake 3.16+（若使用 CMake 构建）
- 支持 C++17 的编译器（GCC 8+/Clang 6+/MSVC 2019+）

### 使用 Qt Creator 编译
1. 打开 `TCP_TcpServer.pro` 文件
2. 选择构建配置（Debug/Release）
3. 点击构建按钮

### 使用 qmake 命令行编译
```bash
cd TCP_Tcpsercer2.0
mkdir build && cd build
qmake ../TCP_TcpServer.pro
make -j$(nproc)
```

### 运行程序
```bash
./TCP_TcpServer
```

## 配置说明

### 默认端口
- 监听地址：`0.0.0.0`（允许所有地址访问）

### 修改监听端口
如需修改监听端口，请在源码中修改相关配置或在运行时指定。

## 注意事项

1. 确保服务器所在网络允许 TCP 连接（关闭防火墙或开放对应端口）
2. 客户端需使用相同的消息协议进行通信
3. 建议在局域网环境下测试，确保网络畅通

## 更新日志

### v2.0
- 重构客户端连接管理机制
- 使用 QUuid 作为客户端唯一标识
- 优化消息传输协议
- 增加多客户端消息转发功能

## 作者

阿叔（陈才坚）
