#include "setspacedialog.h"
#include "ui_setspacedialog.h"

setSpaceDialog::setSpaceDialog(unsigned int space, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::setSpaceDialog)
{
    ui->setupUi(this);
    this->space = space;
    setWindowFlags(windowFlags()&~Qt::WindowMaximizeButtonHint);    // 禁止最大化按钮
    setFixedSize(this->width(),this->height());                     // 禁止拖动窗口大小

    setWindowTitle(tr("Sapce: "));
    Qt::WindowFlags flags=Qt::Dialog;
    flags |=Qt::WindowCloseButtonHint;
    setWindowFlags(flags);

    QString str = QString::number(this->space);
    ui->spinBox->setValue(str.toInt());
    ui->spinBox->setRange(0, 100);
    ui->spinBox->setWrapping(true);
}

setSpaceDialog::~setSpaceDialog()
{
    delete ui;
}

void setSpaceDialog::on_buttonBox_accepted()
{
    this->space = ui->spinBox->value();
}
