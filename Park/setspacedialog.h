#ifndef SETSPACEDIALOG_H
#define SETSPACEDIALOG_H

#include <QDebug>
#include <QDialog>

namespace Ui {
class setSpaceDialog;
}

class setSpaceDialog : public QDialog
{
    Q_OBJECT

public:
    explicit setSpaceDialog(unsigned int space, QWidget *parent = 0);
    ~setSpaceDialog();
    unsigned int space = 0;

private slots:
    void on_buttonBox_accepted();

private:
    Ui::setSpaceDialog *ui;
};

#endif // SETSPACEDIALOG_H
