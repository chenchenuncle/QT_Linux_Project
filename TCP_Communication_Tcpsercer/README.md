# TCP_TcpServer 

基于 Qt 框架的 TCP 服务器应用程序。

## 功能特性

- 支持多客户端连接管理
- 客户端唯一 ID 分配（使用 QUuid）
- 客户端间消息转发
- 实时连接状态显示
- IP 地址与端口信息展示

## 界面预览

<!-- 运行截图 1：服务器启动界面 -->
![服务器启动界面](screenshots/01_server_start.png)

<!-- 运行截图 2：客户端连接成功 -->
![客户端连接](screenshots/02_client_connected.png)

<!-- 运行截图 3：消息收发界面 -->
![消息收发](screenshots/03_message_exchange.png)

## 项目结构

```
TCP_Tcpsercer2.0/
├── main.cpp              # 程序入口
├── mainwindow.h          # 主窗口类声明
├── mainwindow.cpp        # 主窗口类实现
└── TCP_TcpServer.pro     # Qt 项目文件
```

## 核心类说明

### MainWindow

主要业务逻辑类，负责：

- `QTcpServer` - 服务器监听与连接管理
- `QHash<QUuid, QTcpSocket*>` - 客户端连接映射表
- 信号槽处理客户端连接、断开、消息收发等事件

### 关键方法

| 方法 | 功能 |
|------|------|
| `do_newConnection()` | 处理新客户端连接 |
| `do_readyRead()` | 接收并处理客户端消息 |
| `do_disconnected()` | 清理断开的客户端 |
| `Handle()` | 解析消息中的目标客户端 ID |
| `Client_ID()` | 向指定客户端发送消息 |

## 运行要求

- Qt 6.10.2 或更高版本
- 支持 C++17 的编译器

