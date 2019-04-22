#include "usermessage.h"

UserMessage::UserMessage()
{
    /*
    this->uid = "F043B34F";
    this->isComeIn = true;
    this->moneyBalance = 100;
    */
    this->pricePerHour = 5; // 每小时五块，不足一小时按一小时计算
}

// 计算应支付的价格
int UserMessage::caculatePrice()
{
    qDebug() << this->isVip;
    if (this->isVip)
    {
        return 0;
    }
    int time = this->beginTime.secsTo(endTime);

    return 5 * (time / 3600 + (time % 3600 ? 1 : 0)) ;
}

// 进入停车场
void UserMessage::enterInto()
{
    this->isComeIn = false; // 下一状态由进入变为离开
    this->beginTime = QDateTime::currentDateTime(); // 获取开始停车时间
}

// 离开停车场
void UserMessage::leaveOut()
{
    this->isComeIn = true;  // 下一状态由离开变为进入
    this->endTime = QDateTime::currentDateTime();   // 获取离开时间
    this->moneyBalance -= this->caculatePrice();    // 支付停车费
}

// 验证用户身份
bool UserMessage::validateUID(QString qUID)
{
    QXlsx::Document xlsx("UserMessage.xlsx");
    for (int row = 1; ; row++)
    {
        QXlsx::Cell *cell = xlsx.cellAt(row, 1);
        if (cell->value() == VT_NULL)
        {
            break;
        }
        if (cell->value() == qUID)
        {
            this->uid = qUID;
            return true;
        }
    }
    return false;
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
                /*if (xlsx.cellAt(row, 2))
                {
                    this->beginTime = QDateTime::fromString((xlsx.read(row, 2)).value<QString>(), "yyyy/MM/dd hh:mm:ss");
                }
                if (xlsx.cellAt(row, 3))
                {
                    this->endTime = QDateTime::fromString((xlsx.read(row, 3)).value<QString>(), "yyyy/MM/dd hh:mm:ss");
                }
                */
                if (xlsx.cellAt(row, 4))
                {
                    this->moneyBalance = (int)(xlsx.read(row, 4)).value<double>();
                }
                if (xlsx.cellAt(row, 5))
                {
                    this->isComeIn = (int)(xlsx.read(row, 5)).value<double>();
                }
                if (xlsx.cellAt(row, 6))
                {
                    this->isVip = (int)(xlsx.read(row, 6)).value<double>();
                }
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
                //qDebug()<<"Debug: "<<xlsx.cellAt(row, 2)->value();
                //qDebug()<<"Debug: "<<xlsx.cellAt(row, 3)->value();
                //qDebug()<<"Debug: "<<xlsx.cellAt(row, 4)->value();
            }
        }
    }
}
