#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include "QLabel"
#include "QtMqtt/qmqttclient.h"
#include "QJsonObject"
#include "QJsonDocument"
#include "QJsonArray"
#include "QMessageBox"

#include <QSerialPort>
#include <QTimer>
#include <QSerialPortInfo> //串口信息类
#include <QDebug>
#include <QBuffer>
#include <QDate>
#include <QFile>
#include <QFileDialog>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
    void init_mqtt();

    void sendTopic(QString data);

    void ConnectOrDisConnect();

    void Subcribe();

   // void Publish(QString topic);


private slots:
    void on_pushButton_clicked(bool checked);
    void receiveMess(QByteArray message,QMqttTopicName name);
private:
    Ui::Widget *ui;
   QMqttClient *mqtt_client;
    QString str1;
    QString msg;
    QTimer *timer;
    QSerialPort *serialPort;
private slots:
    void readData();
    void slotTimerUpdate();
   // void SaveRecvDataFile();
    void on_pushButton_2_clicked();
    void on_pushButton_4_clicked();

};
#endif // WIDGET_H
