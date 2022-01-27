#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QTimer>
#include <QMessageBox>
#include <QVector>

#define SCAN_SERIAL_TIMER_MS    1000  //1000ms查找一次串口

namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = 0);
    ~Widget();

    void connectInit();          //信号与槽函数
    void serialFind();           //串口扫描函数
    void serialInit(QString PortName , qint32 BaudRate); //串口初始化


    void on_openSerial_clicked();   //打开串口
    void ComRecvData();             //处理串口接收的数据
    void showComecvData();          //显示串口接收到的数据


private slots:
    void on_sendBtn_clicked();

    void on_clearComBtn_clicked();

    void on_cleanSendBtn_clicked();

private:
    Ui::Widget *ui;
    QSerialPort *serialport;          //串口
    bool openSerialFlag = false;      //串口标志
    QStringList PortStringList;       //串口号列表
    QTimer serialScanTimer;           //串口扫描定时器
    QTimer ComCvDataTimer;
    QVector<QByteArray> data;



};

#endif // WIDGET_H
