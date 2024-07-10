#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>

static QString HostName="iot-06z00ap7kgmmcp4.mqtt.iothub.aliyuncs.com";
static quint16 Port = 1883;
static QString username = "qlzy1&k1k10fEOotr" ;
static QString password = "9e64b1646988290cbda2d16f423e1d1c7bb45cbf913a9386c6dbe4a677bb2b61";
static QString ClientId = "k1k10fEOotr.qlzy1|securemode=2,signmethod=hmacsha256,timestamp=1720578643525|";
static QString m_topic_publish= "/sys/izz9L58Kijx/tance/thing/event/property/post";
static QString m_topic_get= "/sys/izz9L58Kijx/tance/thing/event/property/post";

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    timer = new QTimer(this);   //设置一个新的时间用于定时接收数据
    serialPort = new QSerialPort(this);  //新的串口类
    mqtt_client=new QMqttClient;    //mqtt连接
    ConnectOrDisConnect();
   // Publish(m_topic_publish);
    //扫描本机的串口，并且添加到下拉框
    foreach(const QSerialPortInfo &info, QSerialPortInfo::availablePorts()){
    ui->comboBox->addItem(info.portName());
    }

    //读取数据
    connect(serialPort, SIGNAL(readyRead()), this, SLOT(readData()));
    connect(timer,SIGNAL(timeout()),this,SLOT(slotTimerUpdate()));

}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_pushButton_clicked(bool checked)
{

    if (checked){

    //设置要打开的串口的名字
    serialPort->setPortName(ui->comboBox->currentText());
    //设置波特率
    serialPort->setBaudRate(ui->comboBox_2->currentText().toInt());
    //设置停止位
    serialPort->setStopBits(QSerialPort::StopBits(ui->comboBox_3->currentText().toInt()));
    //设置数据位
    serialPort->setDataBits(QSerialPort::DataBits(ui->comboBox_4->currentText().toInt()));
    //设置校验位
    switch (ui->comboBox_5->currentIndex()) {
    case 0:
        serialPort->setParity(QSerialPort::NoParity);
        break;
    case 1:
        serialPort->setParity(QSerialPort::EvenParity);
        break;
    case 2:
        serialPort->setParity(QSerialPort::OddParity);
        break;
    case 3:
        serialPort->setParity(QSerialPort::SpaceParity);
        break;
    case 4:
        serialPort->setParity(QSerialPort::MarkParity);
        break;
    default:
        break;

    }
    //设置流控
    serialPort->setFlowControl(QSerialPort::NoFlowControl);
    if(!serialPort->open(QIODevice::ReadWrite)){
        QMessageBox::about(this,"错误","串口打开失败可能被占用了");
        return;
    }

    ui->comboBox->setEnabled(false);
    ui->comboBox_2->setEnabled(false);
    ui->comboBox_3->setEnabled(false);
    ui->comboBox_4->setEnabled(false);
    ui->comboBox_5->setEnabled(false);
    ui->pushButton->setText("关闭串口");
    }else{
        //关闭串口
        serialPort->close();
        ui->comboBox->setEnabled(true);
        ui->comboBox_2->setEnabled(true);
        ui->comboBox_3->setEnabled(true);
        ui->comboBox_4->setEnabled(true);
        ui->comboBox_5->setEnabled(true);
        ui->pushButton->setText("打开串口");
    }
}
//数据处理阶段
void Widget::readData()
{
    timer->start(50);

    msg.append(serialPort->readAll());

     }

void Widget::slotTimerUpdate()
{
    timer->stop();
    QDateTime time = QDateTime::currentDateTime();//获取当前日期和时间
    QString str = time.toString("yyyy-MM-dd hh:mm:ss dddd");//格式为年-月-日 小时-分钟-秒 星期
    ui->textBrowser_3->setText(str);
    QString wendu;
    QString shidu;
    QString guangzhao;
    if(msg!=' ') //如果数据不为空处理数据
    {
      QString bwendu = "temp";     //因为发送过来的数据有很多
      QString bshidu = "Hum";       //定义几个标识字符串来区别接收到的数据是温度湿度以及其他
      QString bdianliang = "PowerSize";   //发送的的数据为了将数据的分开都带有标识符
      QString byanwu = "SmokeSensor";
      int swendu = msg.indexOf(bwendu,0);   //此处为查询操作定义一个变量来接收查询返回值若为"-1"则没有查询到
      int sshidu = msg.indexOf(bshidu,0);    //通过标识符查找数据
      int sdianliang = msg.indexOf(bdianliang,0);
      int syanwu = msg.indexOf(byanwu,0);
       qDebug() << swendu;
       qDebug() << sshidu;
      if(swendu>=0&&sshidu>=0&&sdianliang>=0&&syanwu>=0)  //判断该数据接收的是否完整
      {
          wendu = msg.mid(swendu+4,2);   //从温度后面截取需要的数据并保存
          ui->textBrowser->setText(wendu);
         int wendua = wendu.toInt();   //将得到的字符型数据转换为整型方便后面保存

          shidu = msg.mid(sshidu+3,2);
          ui->textBrowser_2->setText(shidu);
          int shidua = shidu.toInt();   //定义整型变量方便后面向云平台发送消息

          QString topic = "/sys/izz9L58Kijx/tance/thing/event/property/post";
         // int a = str1.toInt();
          QJsonObject root;     //通过定义一个json数据向云平台发送数据
        //  QJsonObject sys;
          QJsonObject params;
 
          params.insert("temp",wendua);  //向物模型发送数据要有固定的格式
          params.insert("Hum",shidua);
        
          root.insert("id","1354728484");
          root.insert("version","1.0.0");
          //root.insert("sys",sys);
          root.insert("params",params);
          root.insert("methon","thing.service.property.set");

          QJsonDocument document; //定义一个QJsonDocument类读写json格式
          document.setObject(root);
          QByteArray byteArray = document.toJson(QJsonDocument::Compact);
          QString strJson(byteArray);
          qDebug()<<byteArray;
          mqtt_client->publish(topic, byteArray);//mqtt协议发布消息


      }
    }


              // ui->textBrowser_5->insertPlainText(msg);

                msg.clear();

}


//保存数据阶段
void Widget::on_pushButton_2_clicked()
{
    if(str1.size()<1)
    {
        QMessageBox::information(this, "提示","当前数据为空");
        return;
    }
    serialPort->clear();        //清空缓存区
    serialPort->close();        //关闭串口
    QString csvFile = QFileDialog::getExistingDirectory(this);      //获取文件保存路径
    if(csvFile.isEmpty())
       return;
    QDateTime current_date_time =QDateTime::currentDateTime();      //获取系统时间
    QString current_date =current_date_time.toString("MM_dd_hh_mm");    //获取时间字符串
    csvFile += tr("/%1.csv").arg(current_date);
    qDebug()<< csvFile;
    QFile file(csvFile);
    if ( file.exists())
    {
            //如果文件存在执行的操作，此处为空，因为文件不可能存在
    }
    file.open( QIODevice::ReadWrite | QIODevice::Text );    //以读写模式读取文件
    QTextStream out(&file);
    out<<tr("Time,")<<tr("sensor1,")<<tr("sensor2,")<<tr("sensor3,")<<tr("sensor4,")<<tr("sensor5,\n");     //写入表头
    // 创建 CSV 文件
    for (const auto &data : str1) {           //测试格式： *111,222,333,444,555#
        out << data ;        //顺序将缓冲区数据写入文件
    }
    file.close();
         //清空缓存区数据
    QMessageBox::information(this, "提示","数据保存成功");
}
//连接云平台
void Widget::ConnectOrDisConnect()
{

    connect(ui->pushButton_3,&QPushButton::clicked,this,[=](){
        if(mqtt_client->state()==QMqttClient::Disconnected)
        {
            mqtt_client->setHostname(HostName); //设置mqtt连接参数port username password clientid
            mqtt_client->setPort(Port);
            mqtt_client->setUsername(username);
            mqtt_client->setPassword(password);
            mqtt_client->setClientId(ClientId);
            mqtt_client->connectToHost();

            if(mqtt_client->state()==QMqttClient::Disconnected)
            {
                qDebug()<<mqtt_client->error();
            }
            else if(mqtt_client->state()==QMqttClient::Connecting)
            {
                ui->pushButton_3->setText("断开");
                ui->label_11->setText("Connect");
               Subcribe();

            }
        }
        else
        {
            mqtt_client->disconnectFromHost();
            ui->pushButton_3->setText("连接");
            ui->label_11->setText("DisConnect");

        }
    });

}

void Widget::Subcribe()
{
    if(mqtt_client->state()==QMqttClient::Connecting)
    {
        qDebug()<<"connect success";
        mqtt_client->subscribe(m_topic_get); //此处订阅topic主题来接收云平台发送的消息
        //通过连接信号到数据处理阶段处理数据
        connect(mqtt_client, SIGNAL(messageReceived(QByteArray,QMqttTopicName)), this, SLOT(receiveMess(QByteArray,QMqttTopicName)));
    }


}




void Widget::receiveMess(QByteArray message, QMqttTopicName name)
{
    Q_UNUSED(name);
    QString msg=message;  //接收数据
    qDebug()<<msg;
}