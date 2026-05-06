# TCP Socket 客户端应用

一个基于 Qt 框架开发的 TCP Socket 客户端应用程序，支持与服务器建立连接、发送和接收消息。

## 功能特性

- ✅ 启动时输入服务器 IP 地址和端口号
- ✅ 自动连接到指定的 TCP 服务器
- ✅ 支持发送消息到服务器
- ✅ 实时接收并显示服务器返回的消息
- ✅ 显示临时连接 ID
- ✅ 连接状态管理（连接/断开）
- ✅ 错误处理和提示信息

## 运行界面
- **输入ip页面**<img width="1917" height="1077" alt="截图 2026-05-06 21-34-00" src="https://github.com/user-attachments/assets/255162f7-ea70-447d-9c7c-49f1dc20b1b7" />

- **输入端口页面**<img width="1917" height="1077" alt="图片" src="https://github.com/user-attachments/assets/0f4c2e83-ec20-4af7-b7a0-088857a31510" />

- **正式页面** <img width="1917" height="1077" alt="截图 2026-05-06 21-35-03" src="https://github.com/user-attachments/assets/7c9bcdd1-9798-4a4c-8de1-0d5684c6e5c7" />

## 技术栈

- **框架**: Qt 6.10.2
- **语言**: C++17
- **网络模块**: QTcpSocket
- **UI 组件**: QMainWindow, QPlainTextEdit, QLineEdit

## 项目结构

```
TCP_Tcpsocket2.0/
├── main.cpp          # 应用入口，处理IP和端口输入
├── mainwindow.h      # 主窗口头文件
├── mainwindow.cpp    # 主窗口实现
├── mainwindow.ui     # UI 设计文件
└── TCP_Tcpsocket.pro # Qt 项目配置文件
```

## 编译运行

### 环境要求

- Qt 6.x 开发环境
- 支持 C++17 的编译器（MSVC/GCC/Clang）

### 编译步骤

1. 使用 Qt Creator 打开 `TCP_Tcpsocket.pro` 文件
2. 选择合适的构建套件（Desktop Qt 6.x）
3. 点击构建按钮进行编译
4. 运行生成的可执行文件

### 运行说明

1. 启动应用后，会弹出输入对话框
2. 输入服务器 IP 地址（如：127.0.0.1）
3. 输入服务器端口号（如：8888）
4. 应用会自动尝试连接服务器
5. 连接成功后，在输入框中输入消息并发送

## 使用说明

### 界面布局

- **状态栏**: 显示临时 ID 和连接状态
- **消息显示区**: 显示接收的消息和发送的消息
- **输入区**: 
  - 第一行：目标客户端 ID
  - 第二行：要发送的消息内容
- **发送按钮**: 发送消息到服务器

### 菜单操作

- **连接服务器**: 重新连接到服务器
- **断开服务器**: 断开当前连接

## 核心代码解析

### 连接流程

```cpp
// 建立信号槽连接
connect(socket, &QTcpSocket::connected, this, &MainWindow::do_connected);
connect(socket, &QTcpSocket::errorOccurred, this, &MainWindow::do_errorOccurred);
connect(socket, &QTcpSocket::readyRead, this, &MainWindow::do_readyRead);

// 连接到服务器
socket->connectToHost(ip, port);
```

### 消息发送

```cpp
QByteArray array = ui->lineEdit->text().toUtf8() + ':' + ui->lineEdit_2->text().toUtf8();
socket->write(array);
```

### 消息接收

```cpp
QByteArray array = socket->readAll();
// 首次接收的是服务器分配的 ID
if (ID.isNull()) {
    ID = QUuid::fromString(array);
} else {
    ui->plainTextEdit->appendPlainText(array);
}
```

## 注意事项

1. 确保服务器已启动并监听指定端口
2. 输入正确的服务器 IP 地址和端口号
3. 消息格式为 `目标ID:消息内容`
4. 首次连接时，服务器会返回一个临时 ID

## 许可证

本项目仅供学习和参考使用。

--
