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
    bool validateUID(QString qUID);

    QString uid;
    bool isComeIn;
    bool isVip;
    QDateTime beginTime;
    QDateTime endTime;
    int pricePerHour;
    int moneyBalance;
};

#endif // USERMESSAGE_H
