#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    timer = new QTimer(this);
    connect(timer, SIGNAL(timeout()), this, SLOT(timerUpdate()));
    timer->start(1000);

    setWindowTitle(tr("Park"));
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小

    ui->sendButton->setEnabled(false);
    ui->setSpaceButton->setEnabled(false);
    ui->setStatusButton->setEnabled(false);
    ui->barDownButton->setEnabled(false);
    ui->barUpButton->setEnabled(false);

    ui->spaceLabel->setAlignment(Qt::AlignRight);

    userMsg = new UserMessage;
}

MainWindow::~MainWindow()
{
    delete ui;
}

// 非阻塞延时
void MainWindow::Sleep(int msec)
{
	QTime dieTime = QTime::currentTime().addMSecs(msec);
	while (QTime::currentTime() < dieTime)
		QCoreApplication::processEvents(QEventLoop::AllEvents, 100);
}

// 获取并显示时间
void MainWindow::timerUpdate()
{
    QDateTime time = QDateTime::currentDateTime();
    //获取系统现在的时间
    QString str = time.toString("yyyy-MM-dd\nhh:mm:ss dddd");
    //设置系统时间显示格式
    ui->timeLabel->setText(str);
}

// 打开串口
// 使能按钮，向下位机发送获取信息命令（剩余车位、可用状态）
void MainWindow::on_openSerialButton_clicked()
{
    if (ui->openSerialButton->text() == tr("打开串口"))
    {
        serial = new QSerialPort;
        serial->setPortName("COM3");
        //设置波特率
        serial->setBaudRate(QSerialPort::Baud4800);
        //设置数据位数
        serial->setDataBits(QSerialPort::Data8);
        //设置奇偶校验
        serial->setParity(QSerialPort::NoParity);
        //设置停止位
        serial->setStopBits(QSerialPort::OneStop);
        //设置流控制
        serial->setFlowControl(QSerialPort::NoFlowControl);
        //打开串口
        if (!serial->open(QIODevice::ReadWrite))
        {
            QMessageBox::about(NULL, "错误", "无法打开串口!     ");
            return;
        }
        else
        {
			
            // Display time.
            QDateTime time = QDateTime::currentDateTime();
            QString str = ui->textBrowser->toPlainText();
            str += time.toString("yyyy-MM-dd hh:mm:ss\n");
            ui->textBrowser->clear();
            ui->textBrowser->append(str);

            // Button enable
            ui->openSerialButton->setText(tr("关闭串口"));
            ui->sendButton->setEnabled(true);
            ui->setSpaceButton->setEnabled(true);
            ui->setStatusButton->setEnabled(true);
            ui->barDownButton->setEnabled(true);
            ui->barUpButton->setEnabled(true);

			serial->write("0GETMSG\n");
			Sleep(20);
			QObject::connect(serial, &QSerialPort::readyRead, this, &MainWindow::readData);
        }
    }
    else
    {
        //关闭串口
        serial->clear();
        serial->close();
        serial->deleteLater();
        //恢复设置使能
        ui->openSerialButton->setText(tr("打开串口"));
        ui->sendButton->setEnabled(false);
        ui->setSpaceButton->setEnabled(false);
        ui->setStatusButton->setEnabled(false);
        ui->barDownButton->setEnabled(false);
        ui->barUpButton->setEnabled(false);
    }
}

// 从串口接收数据
void MainWindow::readData()
{
    QByteArray buf;
    buf = serial->readAll();
    if (tr(buf) == "OK\n")
    {
        recv = tr(buf);
    }

    if(!buf.isEmpty())
    {
        QString str = ui->textBrowser->toPlainText();
        str += tr(buf);
        ui->textBrowser->clear();
        ui->textBrowser->append(str);
    }
    if (buf[0] == 'M')
    {
        getMessage(tr(buf));
    }
    if (buf.mid(0, 4) == "UID:")
    {   // 检测到刷卡动作
        readUID(buf.mid(4, 8));
    }
    buf.clear();
}

void MainWindow::readUID(QString comm)
{
    if (userMsg->validateUID(comm))  // 验证UID
    {
//         /*qDebug()*/        qDebug() << "UID: " << userMsg->uid << endl;
        userMsg->getMsgFromFile();
        if (userMsg->isComeIn)      // enter into
        {
            userMsg->enterInto();
            QString str = ui->textBrowser->toPlainText();
            str +=  tr("Begin: ") + userMsg->beginTime.toString("yyyy-MM-dd hh:mm:ss") + tr("\n");
            ui->textBrowser->clear();
            ui->textBrowser->append(str);

            serial->write("5\n");
            Sleep(15);
        }
        else    // leave out
        {
            userMsg->leaveOut();
            QString str = ui->textBrowser->toPlainText();
            str += tr("From: ") + userMsg->beginTime.toString("yyyy-MM-dd hh:mm:ss") + tr("\n");
            str += tr("To: ") + userMsg->endTime.toString("yyyy-MM-dd hh:mm:ss") + tr("\n");
            str += tr("Price: ") + QString::number(userMsg->caculatePrice()) + tr(" Balance: ")
                    + QString::number(userMsg->moneyBalance) + tr("\n");
            ui->textBrowser->clear();
            ui->textBrowser->append(str);

            char tmp[20];
            sprintf(tmp, "6  P:%03d B:%04d\n", userMsg->caculatePrice(), userMsg->moneyBalance);
            serial->write(tmp);
            Sleep(15);
       }
       userMsg->saveMsgToFile();
    }
}

// 解析从下位机接收到的信息
// 车位数量、可用状态
void MainWindow::getMessage(QString comm)
{
    if (comm[1] == '0')
    {   // 第二位数据表示可用状态
        this->isAllow = false;
        ui->statusLabel->setText(tr("停止使用"));
    }
    else
    {
        this->isAllow = true;
        ui->statusLabel->setText(tr("正常使用"));
    }
    // 从第三位起的三位数表示车位数量
    QString tmp = comm.mid(2, 3);
    this->capacity = tmp.toInt();
    ui->spaceLabel->setText(QString::number(capacity));
}

// 自定义文本框指令发送到下位机
void MainWindow::on_sendButton_clicked()
{
	//QByteArray str = ui->textEdit->toPlainText().toLatin1() + "\n";
    
	serial->write(ui->textEdit->toPlainText().toLatin1() + "\n");
	Sleep(15);
	/*if (recv == "ok\n")
	{
		serial->write("0getmsg\n");
	}*/
}

// 清除文本显示
void MainWindow::on_clearButton_clicked()
{
    ui->textBrowser->clear();
}

// 设置空余车位
void MainWindow::on_setSpaceButton_clicked()
{
    spaceDialog = new setSpaceDialog(this->capacity, this);
    spaceDialog->exec();

    // Send com to MCU51
    QString str = "1" + QString::number(spaceDialog->space) + "\n";
    serial->write(str.toStdString().data());
	Sleep(15);
    if (recv == "OK\n" && spaceDialog->space <= this->maxCapacity)
    {
        recv = "    ";
        capacity = spaceDialog->space;
        ui->spaceLabel->setText(QString::number(capacity));
    }
}

// 设置可用状态
void MainWindow::on_setStatusButton_clicked()
{
    serial->write("2SET\n");
	Sleep(15);
	if (recv == "OK\n")
	{
		if (ui->statusLabel->text() == tr("正常使用"))
		{
			ui->statusLabel->setText(tr("停止使用"));
		}
		else if (ui->statusLabel->text() == tr("停止使用"))
		{
			ui->statusLabel->setText(tr("正常使用"));
		}
		recv = "   ";
	}
}

// 抬杆
void MainWindow::on_barUpButton_clicked()
{
    serial->write("3\n");
    Sleep(15);
}

// 降杆
void MainWindow::on_barDownButton_clicked()
{
    serial->write("4\n");
    Sleep(15);
}
