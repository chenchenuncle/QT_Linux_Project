# ESP8266(可适用别的wifi开发版) 串口助手

基于 Qt 框架开发的功能完善的串口通信工具，专门为 ESP8266 WiFi 模块 AT 指令调试设计。

## 📋 项目简介

这是一个跨平台的串口调试助手，提供直观的图形界面用于：
- 串口通信参数配置与管理
- ESP8266 AT 指令的快捷发送与响应查看
- WiFi 模式配置与网络连接管理
- TCP/UDP 通信与透传模式支持
- 快速调试与开发 ESP8266 模块

## ✨ 主要功能

### 串口管理
- 自动扫描可用串口
- 支持多种波特率配置（9600 - 230400）
- 可配置数据位（5-8）、停止位（1/1.5/2）、校验位
- 实时数据收发显示与记录
- 串口状态实时监测与错误提示

### ESP8266 AT 指令操作
**模块基础操作：**
- 模块测试（AT）
- 模块重启（AT+RST）
- 恢复出厂设置（AT+RESTORE）

**WiFi 串口配置：**
- 查询/设置串口参数（AT+UART_CUR）

**WiFi 网络设置：**
- WiFi 模式设置（STA/AP/STA+AP）
- 查询/连接 WiFi 热点
- 查询 IP 地址与 MAC 地址

**TCP 服务器配置：**
- TCP 连接建立
- 服务器参数配置
- 查询连接状态
- 透传模式（进入/退出）
- 开机自动连接透传设置

### 快捷 AT 指令
内置常用 AT 指令模板，支持一键发送，包括：
- AT+CWMODE - WiFi 模式
- AT+UART - 串口参数
- AT+CIPSTART - 建立连接
- AT+CIPSEND - 发送数据
- 以及更多常用指令

## 🛠️ 技术栈

- **框架**: Qt 6.x (Qt SerialPort, Qt Widgets)
- **语言**: C++17
- **构建工具**: qmake / CMake
- **开发平台**: Linux (Fedora)
- **支持平台**: Windows, Linux, macOS (跨平台)

## 📁 项目结构

```
serial_port_assistant/
├── main.cpp              # 程序入口点
├── mainwindow.h          # 主窗口头文件
├── mainwindow.cpp        # 主窗口实现
├── mainwindow.ui         # Qt Designer UI 文件
├── serial_port_assistant.pro  # qmake 项目配置
├── qrc.qrc               # Qt 资源文件
└── qrc/                  # 资源目录（图片等）
    ├── 123.jpg
    └── 828.bmp
```

## 🚀 快速开始

### 环境要求

- Qt 6.x（建议 6.10.2 或更高版本）
- C++17 兼容编译器（GCC 8+ / MSVC 2017+ / Clang 6+）
- qmake 或 CMake 构建工具
- 串口驱动（CH340, CP2102 等 USB 转串口驱动）

### 编译步骤

#### 方法一：使用 Qt Creator
1. 打开 Qt Creator
2. 打开 `serial_port_assistant.pro` 项目文件
3. 配置构建套件（Kits）
4. 点击运行或按 `Ctrl+R`

#### 方法二：命令行编译

```bash
# 进入项目目录
cd serial_port_assistant

# 创建构建目录（可选，建议使用）
mkdir build && cd build

# 使用 qmake 生成 Makefile
qmake ../serial_port_assistant.pro

# 编译（Linux/macOS）
make -j$(nproc)

# 编译（Windows MinGW）
mingw32-make

# 运行程序
./serial_port_assistant
```

## 📖 使用说明

### 1. 打开串口
1. 从串口号下拉列表选择设备（如 `/dev/ttyUSB0` 或 `COM3`）
2. 配置串口参数（默认 115200, 8, N, 1）
3. 点击"打开串口"按钮
4. 观察日志窗口确认连接成功

### 2. ESP8266 模块操作
#### 模块初始化
- 点击"模块测试"发送 `AT`，应返回 `OK`
- 如需恢复出厂，点击"恢复出厂"

#### WiFi 连接
1. 配置 WiFi 模块为 Station 模式
2. 在 WiFi 连接面板输入 SSID 和密码
3. 点击连接按钮
4. 查看返回的 IP 地址

#### TCP 通信
1. 在 TCP 服务器参数面板输入服务器 IP 和端口
2. 点击"设置服务器参数"
3. 连接成功后点击"进入透传模式"
4. 发送数据测试

### 3. 快捷 AT 指令
使用"常用 AT 指令"标签页的预设按钮快速发送常用命令。

## 🔧 串口参数配置

| 参数 | 默认值 | 可选值 |
|------|--------|--------|
| 波特率 | 115200 | 9600, 19200, 38400, 57600, 115200, 230400 |
| 数据位 | 8 | 5, 6, 7, 8 |
| 停止位 | 1 | 1, 1.5, 2 |
| 校验位 | None | None, Odd, Even, Mark, Space |

## 💡 常见问题

### Linux 串口权限问题
如果在 Linux 下提示权限被拒绝，需要将用户添加到 `dialout` 组：
```bash
sudo usermod -a -G dialout $USER
```
然后注销并重新登录。

### ESP8266 波特率
ESP8266 出厂默认波特率为 115200。如果修改过，需要使用相应的波特率连接。

### 进入透传模式失败
确保：
1. 已成功建立 TCP 连接
2. 发送 `AT+CIPSEND` 前连接处于活跃状态
3. 如需退出透传，发送 `+++`（需要延迟）

## 📚 参考文档

- [ESP8266 AT 指令集官方文档](https://www.espressif.com/zh-hans/support/download/documents)
- [Qt SerialPort 文档](https://doc.qt.io/qt-6/qserialport.html)

## 📄 许可证

本项目使用 MIT 许可证，详见 [LICENSE](LICENSE) 文件。

## 🤝 贡献

欢迎贡献代码、报告问题或提出改进建议！

---

**注意**: 本项目仅供学习和开发使用。ing README.md…]()
