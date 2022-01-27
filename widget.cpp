#include "widget.h"
#include "ui_widget.h"


Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    // 1. 设置窗口标题
    this->setWindowTitle("自制串口助手");
    // 2. 设置窗口图标
    //setWindowIcon(":/image.qrc");

    // 3. 给串口分配内存空间
    serialport = new QSerialPort(this);

    // 4. 信号与槽函数connectInit
    connectInit();

    //开启扫面串口定时器
    serialScanTimer.start(SCAN_SERIAL_TIMER_MS);

}

Widget::~Widget()
{
    delete ui;
}
/***************** 信号与槽函数 ******************************/
void Widget::connectInit()
{
    //扫描串口信号槽
    connect(&serialScanTimer,&QTimer::timeout,
            [=]()
            {
               //查找串口，更新下拉表
               serialFind();
            }
            );
    //打开串口
    connect(ui->openSerialBtn,&QPushButton::clicked,
            [=]()
            {
                on_openSerial_clicked();
            }
            );
    //读取串口数据槽函数 下位机有消息发来 就会触发此信号
    connect(serialport,&QSerialPort::readyRead,
            [=]()
            {
              ComRecvData();
            }
            );
    //显示串口数据信号槽
    connect(&ComCvDataTimer,&QTimer::timeout,
            [=]()
            {
              showComecvData();
            }
            );

}
/***************** 串口扫描 ********************************/
void Widget::serialFind()
{
    QStringList newPortStringList;

    //遍历串口 自动寻找可用串口
    foreach(const QSerialPortInfo &info ,QSerialPortInfo::availablePorts())
        newPortStringList += info.portName();

    if(newPortStringList.size() != PortStringList.size())
    {
        PortStringList = newPortStringList;
        ui->serialportBox->clear();
        ui->serialportBox->addItems(PortStringList);
    }

    //遍历串口 自动寻找可用串口
//    foreach(const QSerialPortInfo &info ,QSerialPortInfo::availablePorts())

//    serialport->setPort(info);
//    if(serialport->open(QIODevice::ReadWrite))
//    {
//        ui->serialportBox->addItems(serialport->portName());
//        serialport->close();
//    }

}
/***************** 串口初始化 ******************************/
void Widget::serialInit( QString PortName , qint32 BaudRate)
{
    //设置串口号
     serialport->setPortName(PortName);
    //设置波特率
    serialport->setBaudRate(BaudRate);
    //设置停止位
    serialport->setStopBits(QSerialPort::OneStop);
    ui->stopBox->addItem("1");
    //设置校验位
    serialport->setParity(QSerialPort::NoParity);
    ui->parityBox->addItem("None");
    //设置数据位
    serialport->setDataBits(QSerialPort::Data8);
     ui->dataBox->addItem("8");
    //设置流控制
    serialport->setFlowControl(QSerialPort::NoFlowControl);
    ui->flowcontrolBox->addItem("None");
}
/*****************  ******************************/

/***************** 接收数据 ********************************/
void Widget::ComRecvData()
{

    // 启动定时器，接收100ms数据（根据情况设定）
    ComCvDataTimer.start(2);
    data.reserve(500);
    data.append(serialport->readAll());

}
/***************** 显示串口接收到的数据 ********************************/
void Widget::showComecvData()
{
    ComCvDataTimer.stop();
    if(data.empty()){
        ui->ComCvEdit->append("串口接收数据长度为0");
    }
    // 将接收到数据显示到
    for(int i = 0 ; i < data.size() ;i++){
        QString StrComCvData;

        StrComCvData = QString::fromLocal8Bit(data.at(i).toHex());
        ui->ComCvEdit->insertPlainText(StrComCvData);
        ui->ComCvEdit->moveCursor(QTextCursor::End);
            }
    data.clear();
}

/***************** 打开串口 ********************************/
void Widget::on_openSerial_clicked()
{
    QString portName ;
    portName = ui->serialportBox->currentText();

    qint32 baudRate;
    baudRate = ui->baudBox->currentText().toInt();

    serialInit(portName,baudRate);

    if(openSerialFlag == false){
        if(!serialport->open(QIODevice::ReadWrite)){
             QMessageBox::information(this,"提示","串口打开失败，请检查串口是否正常连接");
            }else{
             QMessageBox::information(this,"提示","串口打开成功！");
             ui->openSerialBtn->setText("关闭串口");
            openSerialFlag = true;
            }
    }else{
        serialport->close();
         QMessageBox::information(this,"提示","串口关闭成功");
         openSerialFlag = false;
    }
}

void Widget::on_sendBtn_clicked()
{
   serialport->write(ui->SendEdit->toPlainText().toLatin1());
}

void Widget::on_clearComBtn_clicked()
{
    ui->ComCvEdit->clear();
}

void Widget::on_cleanSendBtn_clicked()
{
    ui->SendEdit->clear();
}
