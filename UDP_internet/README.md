# UDP 互联网通信工具

## 项目简介

这是一个基于 Qt 6.10.2 开发的 UDP 通信应用程序，支持**单播**、**广播**和**组播**三种通信模式。应用程序能够自动获取本机 IPv4 地址，并提供友好的图形界面进行网络通信测试。

## 主要功能

### 1. 单播通信（Unicast）
- 向指定 IP 地址和端口发送点对点消息
- 实时接收来自特定目标的消息
- 消息显示在左侧文本框中

### 2. 广播通信（Broadcast）
- 向当前网络段所有设备广播消息
- 不需要指定具体目标 IP
- 适合局域网内的信息发布

### 3. 组播通信（Multicast）
- 加入指定的组播组地址
- 向组播组成员发送消息
- 消息显示在右侧文本框中
- 支持动态更改组播地址

## 运行界面

<img width="1917" height="1077" alt="图片" src="https://github.com/user-attachments/assets/675839e7-7c24-469d-83a3-1ceb89075bb8" /

## 界面组件说明

### 顶部控制栏
| 组件 | 类型 | 功能说明 |
|------|------|----------|
| 端口 | QSpinBox | 显示和修改本地监听端口（范围：0-999999） |
| 目标端口 | QSpinBox | 设置消息发送目标端口（范围：0-999999） |
| 目标地址 | QComboBox | 输入或选择目标 IP 地址（可编辑下拉框） |

### 单播/广播消息区（左侧）
| 组件 | 类型 | 功能说明 |
|------|------|----------|
| lineEdit | QLineEdit | 输入要发送的单播/广播消息内容 |
| pushButton | QPushButton | 发送单播消息到指定目标地址 |
| pushButton_3 | QPushButton | 发送广播消息到所有设备 |
| plainTextEdit | QPlainTextEdit | 显示单播和广播消息记录 |

### 组播控制区（右侧）
| 组件 | 类型 | 功能说明 |
|------|------|----------|
| 组播地址 | QComboBox | 设置组播组地址（可编辑，支持动态修改） |
| lineEdit_2 | QLineEdit | 输入组播消息内容 |
| pushButton_2 | QPushButton | 发送组播消息到指定组播组 |
| plainTextEdit_2 | QPlainTextEdit | 显示组播消息记录 |

### 工具栏
- **清空文本框**: 清除所有消息显示区域的文字
- **退出**: 关闭应用程序

## 技术实现

### 核心技术栈
- **Qt 6.10.2**: 跨平台 C++ 图形用户界面库
- **QUdpSocket**: Qt 提供的 UDP 套接字类
- **QNetworkDatagram**: Qt 封装的数据报类
- **QNetworkInterface**: 网络接口信息获取
- **Qt Meta-Object System**: 信号与槽机制

### 关键实现

#### UDP 套接字初始化
```cpp
udpsocket = new QUdpSocket(this);

// 自动获取本机 IPv4 全局地址
QList<QHostAddress> listIPv4 = QNetworkInterface::allAddresses();
for(const auto &i : listIPv4)
    if(i.isGlobal() && i.protocol() == QAbstractSocket::IPv4Protocol)
    {
        this->setWindowTitle(i.toString()); // 窗口标题显示 IP
        break;
    }

// 绑定本地端口
udpsocket->bind(QHostAddress::Any, port);
```

#### 单播消息发送
```cpp
void MainWindow::on_pushButton_clicked()
{
    QHostAddress ip(ui->comboBox->currentText());
    int port = ui->spinBox->value();
    QByteArray array = ui->lineEdit->text().toUtf8();

    QNetworkDatagram datagram(array, ip, port);
    udpsocket->writeDatagram(datagram);

    ui->plainTextEdit->appendPlainText("我：" + ui->lineEdit->text());
    ui->lineEdit->clear();
}
```

#### 广播消息发送
```cpp
void MainWindow::on_pushButton_3_clicked()
{
    QHostAddress ip(ui->comboBox->currentText());
    int port = ui->spinBox->value();
    QByteArray array = ui->lineEdit->text().toUtf8();

    QNetworkDatagram datagram(array, QHostAddress::Broadcast, port);
    udpsocket->writeDatagram(datagram);

    ui->plainTextEdit->appendPlainText("我:" + ui->lineEdit->text());
    ui->lineEdit->clear();
}
```

#### 组播消息处理
```cpp
void MainWindow::on_pushButton_2_clicked()
{
    QNetworkDatagram data;
    data.setDestination(Multicast, ui->spinBox->value());
    data.setData(ui->lineEdit_2->text().toUtf8());

    udpsocket->writeDatagram(data);

    ui->plainTextEdit_2->appendPlainText("我" + ui->lineEdit_2->text());
    ui->lineEdit_2->clear();
}
```

#### 接收消息处理
```cpp
void MainWindow::do_readyRead()
{
    QNetworkDatagram datagram = udpsocket->receiveDatagram();
    QHostAddress senderIP = datagram.senderAddress();
    QByteArray data = senderIP.toString().toUtf8() + ':' + datagram.data();

    QHostAddress destIP = datagram.destinationAddress();

    if(destIP == QHostAddress::Broadcast) {
        ui->plainTextEdit->appendPlainText("(广播)" + data);
    } else if(destIP.toString() == this->windowTitle()) {
        ui->plainTextEdit->appendPlainText(data);
    } else {
        ui->plainTextEdit_2->appendPlainText(data);
    }
}
```

#### 动态组播地址切换
```cpp
bool MainWindow::eventFilter(QObject *object, QEvent *event)
{
    if(object == ui->comboBox_2 && event->type() == QEvent::FocusOut)
    {
        udpsocket->leaveMulticastGroup(Multicast);
        Multicast.setAddress(ui->comboBox_2->currentText());
        udpsocket->joinMulticastGroup(Multicast);
    }
    return QMainWindow::eventFilter(object, event);
}
```

## 使用说明

### 1. 单播通信
1. 在"目标地址"下拉框中输入目标设备 IP（如 192.168.1.100）
2. 设置"目标端口"号（确保对方也监听此端口）
3. 在消息输入框中输入内容
4. 点击"发送消息"按钮
5. 消息将显示在左侧消息区域

### 2. 广播通信
1. 设置"目标端口"号
2. 在消息输入框中输入内容
3. 点击"广播消息"按钮
4. 同一网络段的所有设备都能收到消息（显示为"(广播)"开头）

### 3. 组播通信
1. 在"组播地址"栏设置组播组地址
   - 常用组播地址：224.0.0.1（所有主机）、224.0.0.2（所有路由器）
   - 有效范围：224.0.0.0 - 239.255.255.255
2. 设置"目标端口"号
3. 在消息输入框中输入内容
4. 点击"群播消息"按钮
5. 组播组内的所有成员都能收到消息

### 4. 修改本地端口
1. 在"端口" spinBox 中输入新端口号
2. 按下回车键确认
3. 系统会自动重新绑定新端口
4. 如果端口被占用，会提示错误并随机分配

## 项目结构

```
UDP_internet2.0/
├── main.cpp              # 应用程序入口文件
├── mainwindow.h          # 主窗口类声明
├── mainwindow.cpp        # 主窗口类实现
├── mainwindow.ui         # Qt Designer UI 界面文件
└── UDP_internet.pro      # qmake 项目配置文件
```

## 编译运行

### 前提条件
- Qt 6.10.2 或更高版本
- Qt Creator IDE（推荐）
- 支持 C++17 的编译器
- 构建目录：`build/Desktop_Qt_6_10_2-Debug/`

### 编译步骤
```bash
# 进入项目目录
cd /home/chen_caijian/Qt_Project/UDP_internet2.0

# 使用 qmake 生成 Makefile
qmake UDP_internet.pro

# 编译项目
make

# 运行程序
./UDP_internet
```

### 已编译版本
项目已编译完成，可直接运行：
```bash
./build/Desktop_Qt_6_10_2-Debug/UDP_internet
```

## 注意事项

1. **防火墙设置**: 确保目标设备的防火墙允许 UDP 通信，否则可能收不到消息

2. **端口占用**: 如果端口被占用，系统会自动提示并随机分配可用端口

3. **组播地址**: 有效的组播地址范围是 224.0.0.0 到 239.255.255.255
   - 224.0.0.0 - 224.0.0.255: 永久组播组（不建议使用）
   - 224.0.1.0 - 238.255.255.255: 临时组播组（可正常使用）
   - 239.0.0.0 - 239.255.255.255: 私有组播地址（局域网使用）

4. **网络权限**: 确保应用程序具有网络访问权限

5. **消息显示格式**:
   - 单播消息：`发送者IP:消息内容`
   - 广播消息：`(广播)发送者IP:消息内容`
   - 组播消息：`发送者IP:消息内容`

## 适用场景

- 局域网内的即时通讯和消息推送
- 网络调试和协议测试工具
- 分布式系统的消息广播
- 多播组通信测试和开发
- 物联网设备的消息推送
- 教育培训网络通信原理
