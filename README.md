# QT_Linux_Project

本仓库收集了若干基于 Qt 的 Linux 项目与示例，覆盖桌面与嵌入式设备的图形界面、设备控制、网络与串口通信、多媒体、以及工具类应用。每个子文件夹通常是一个独立的小项目或示例，便于复用、演示与移植到不同的 Linux 平台（例如 x86、ARM 或基于 Yocto/Buildroot 的系统）。

简要说明：这些项目多采用 C++ 与 Qt（Qt Widgets 或 QML），并结合常见的网络/串口/多媒体库与系统服务进行开发，适合作为学习参考或作为嵌入式/桌面应用的起点。

## 目录结构（示例）
- TCP_Communication_Tcpsercer/ — TCP 服务器示例（多客户端、消息转发、GUI）
- TCP_Communication_Tcpsocket/ — TCP 客户端示例（连接、收发消息、GUI）
- EmbeddedControlPanel/      — 示例：嵌入式设备控制面板（可选）
- qt-serial-tool/            — 串口通信与调试工具
- network-gui/               — 网络配置与状态展示面板
- libs/                      — 公共库（跨项目复用）
- docs/                      — 文档、设计说明与开发笔记


## 示例项目：EmbeddedControlPanel（示例）
下面以 "EmbeddedControlPanel" 为例，说明典型项目的功能、技术栈与模块划分：

功能简介：
- 在嵌入式 Linux 设备上展示系统/设备状态与运行信息。
- 提供设备配置、控制命令（如启动/停止服务）与日志查看功能。
- 支持本地串口与网络通信（HTTP/REST、MQTT），可与后端服务或其他设备交互。

使用技术：
- Qt（Qt 6 或 Qt 5，可根据需要选择）
  - 界面实现：Qt Widgets 或 QML（取决于 UI 需求与性能要求）
- C++17/20：核心业务逻辑与底层交互
- 构建系统：CMake（推荐），仓库中亦可能包含 qmake 示例
- 交叉编译支持：Yocto / Buildroot 工具链配置示例
- 通信库与协议：
  - 串口：QtSerialPort 或 termios
  - 网络：Qt Network / libcurl
  - 消息：MQTT 客户端（Paho / Mosquitto）
  - D-Bus：与 systemd、NetworkManager 等系统服务交互
- 数据存储：SQLite（配置与历史数据持久化）
- 安全：OpenSSL（TLS），以及针对嵌入式平台的应用签名/权限控制
- 测试：Qt Test / Google Test
- CI：GitHub Actions（自动构建、测试、打包）

典型模块划分：
- src/ 或 app/
  - core/    — 业务逻辑、状态机与控制策略
  - ui/      — QML/Widgets 界面层、ViewModel（MVVM）或控制器
  - hw/      — 硬件抽象层（串口、GPIO、I2C、SPI 等）
  - net/     — 网络客户端、REST 调用、MQTT 接入
  - storage/ — 配置管理、SQLite 封装
  - utils/   — 日志、配置解析、通用工具
- res/       — 图标、布局、翻译文件（.ts/.qm）
- tests/     — 单元测试与集成测试
- build/     — 构建输出目录（通常被 .gitignore 忽略）


## 其他仓库中示例项目（快速概览）
- TCP_Communication_Tcpsercer：基于 QtNetwork 的 TCP 服务端，支持多客户端连接管理、消息转发与 GUI 展示（详见子目录 README）。
- TCP_Communication_Tcpsocket：基于 QTcpSocket 的客户端示例，支持连接、发送/接收消息与简单 UI。


## 快速开始（开发者参考）
1. 安装 Qt SDK（Qt 5/6）、CMake 与编译器（gcc/clang/msvc）。
2. 克隆仓库：
   git clone https://github.com/chenchenuncle/QT_Linux_Project.git
3. 进入某个示例项目（例如 TCP_Communication_Tcpsocket 或 EmbeddedControlPanel），阅读该子目录下的 README 获取编译/运行说明。
4. 常见本地构建步骤（示例，若项目使用 CMake）：
   mkdir build && cd build
   cmake ..
   make -j$(nproc)
   ./YourApp
5. 若为嵌入式目标，请参考 docs/ 下的 Yocto/Buildroot 示例进行交叉编译配置。


## 贡献指南与约定
- 代码风格：建议使用 clang-format / cpplint（若仓库中有配置文件请遵循）。
- 分支策略：基于主/默认分支创建 feature/bugfix 分支并通过 Pull Request 提交修改。
- 提交说明：提交信息应包含变更摘要与必要的上下文（问题号、设计说明）。
- 测试覆盖：新增功能应配套单元测试与集成测试。
