#ifndef USERMESSAGE_H
#define USERMESSAGE_H

#include <QtCore>
#include <QTimer>
#include <QtXlsx>

class UserMessage
{
public:
    UserMessage();
    void enterInto();
    void leaveOut();
    int caculatePrice();
    void getMsgFromFile();
    void saveMsgToFile();
    bool validateUID(QByteArray buf);

    QString uid;
    bool isComeIn;
    QDateTime beginTime;
    QDateTime endTime;
    int pricePerHour;
    int moneyBalance;
};

#endif // USERMESSAGE_H
