#include "usermessage.h"

UserMessage::UserMessage()
{
    this->uid = "F043B34F";
    this->isComeIn = true;
    this->moneyBalance = 100;
    this->pricePerHour = 5;
}

int UserMessage::caculatePrice()
{
    int time = this->beginTime.secsTo(endTime);

    return 5 * (time / 3600 + (time % 3600 ? 1 : 0)) ;
}

void UserMessage::enterInto()
{
    this->isComeIn = false;
    this->beginTime = QDateTime::currentDateTime();

}

void UserMessage::leaveOut()
{
    this->isComeIn = true;
    this->endTime = QDateTime::currentDateTime();
    this->moneyBalance -= this->caculatePrice();
}

// 获取当前UID信息
void UserMessage::getMsgFromFile()
{
    QXlsx::Document xlsx("UserMessage.xlsx");

    for (int row=1; row<10; ++row)
    {
        if (QXlsx::Cell *cell = xlsx.cellAt(row, 1))
        {
            if (cell->value() == this->uid)
            {
             // QVariant variant = *(xlsx.cellAt(row, 2).value());
                this->beginTime = (xlsx.cellAt(row, 2)->value()).value<QDateTime>();
                this->endTime = (xlsx.cellAt(row, 3)->value()).value<QDateTime>();;
                this->moneyBalance = (xlsx.cellAt(row, 3)->value()).value<int>();
                this->isComeIn = (xlsx.cellAt(row, 3)->value()).value<int>();
                qDebug() << row << this->beginTime << this->endTime << this->moneyBalance;
            }
        }
    }
}

// 存储当前UID信息
void UserMessage::saveMsgToFile()
{
    QXlsx::Document xlsx("UserMessage.xlsx");
    for (int row=1; row<10; ++row)
    {
        if (QXlsx::Cell *cell=xlsx.cellAt(row, 1))
        {
            if (cell->value() == this->uid)
            {
                QString qstr = this->beginTime.toString("yyyy/MM/dd hh:mm:ss");
                xlsx.write(row, 2, QVariant(qstr));
                qstr = this->endTime.toString("yyyy/MM/dd hh:mm:ss");
                xlsx.write(row, 3, QVariant(qstr));
                xlsx.write(row, 4, QVariant(this->moneyBalance));
                if (this->isComeIn)
                {
                    xlsx.write(row, 5, 1);
                }
                else
                {
                    xlsx.write(row, 5, 0);
                }
                xlsx.save();
                qDebug()<<"Debug: "<<xlsx.cellAt(row, 2)->value();
                qDebug()<<"Debug: "<<xlsx.cellAt(row, 3)->value();
                qDebug()<<"Debug: "<<xlsx.cellAt(row, 4)->value();
            }
        }
    }
}

bool UserMessage::validateUID(QByteArray buf)
{
    if ("C551A2AB" == buf.mid(4, 8) || "F043B34F" == buf.mid(4, 8))
    {
        return true;
    }
    else
    {
        return false;
    }
}
