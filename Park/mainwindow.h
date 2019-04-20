#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QtCore>
#include <QTimer>
#include <QMainWindow>
#include <QMessageBox>
#include <QtSerialPort/QSerialPort>
#include <QtSerialPort/QSerialPortInfo>
#include "setspacedialog.h"
#include "usermessage.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    QTimer *timer;
    unsigned int capacity;
    unsigned int maxCapacity = 99;
    bool isAllow = false;
    setSpaceDialog *spaceDialog;
    QString recv;
    UserMessage *userMsg;

    void Sleep(int msec);

private slots:
    void readData();
    void getMessage(QString comm);
    void readUID(QString comm);
    void timerUpdate();
    void on_openSerialButton_clicked();

    void on_sendButton_clicked();

    void on_clearButton_clicked();

    void on_setSpaceButton_clicked();

    void on_setStatusButton_clicked();

    void on_barUpButton_clicked();

    void on_barDownButton_clicked();

private:
    Ui::MainWindow *ui;
    QSerialPort *serial;
};

#endif // MAINWINDOW_H
