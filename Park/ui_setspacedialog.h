/********************************************************************************
** Form generated from reading UI file 'setspacedialog.ui'
**
** Created by: Qt User Interface Compiler version 5.10.0
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_SETSPACEDIALOG_H
#define UI_SETSPACEDIALOG_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QDialog>
#include <QtWidgets/QDialogButtonBox>
#include <QtWidgets/QGridLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QSpinBox>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_setSpaceDialog
{
public:
    QWidget *layoutWidget;
    QGridLayout *gridLayout;
    QSpinBox *spinBox;
    QDialogButtonBox *buttonBox;

    void setupUi(QDialog *setSpaceDialog)
    {
        if (setSpaceDialog->objectName().isEmpty())
            setSpaceDialog->setObjectName(QStringLiteral("setSpaceDialog"));
        setSpaceDialog->resize(215, 108);
        layoutWidget = new QWidget(setSpaceDialog);
        layoutWidget->setObjectName(QStringLiteral("layoutWidget"));
        layoutWidget->setGeometry(QRect(10, 10, 195, 86));
        gridLayout = new QGridLayout(layoutWidget);
        gridLayout->setObjectName(QStringLiteral("gridLayout"));
        gridLayout->setContentsMargins(0, 0, 0, 0);
        spinBox = new QSpinBox(layoutWidget);
        spinBox->setObjectName(QStringLiteral("spinBox"));

        gridLayout->addWidget(spinBox, 0, 0, 1, 1);

        buttonBox = new QDialogButtonBox(layoutWidget);
        buttonBox->setObjectName(QStringLiteral("buttonBox"));
        buttonBox->setOrientation(Qt::Horizontal);
        buttonBox->setStandardButtons(QDialogButtonBox::Cancel|QDialogButtonBox::Ok);

        gridLayout->addWidget(buttonBox, 1, 0, 1, 1);


        retranslateUi(setSpaceDialog);
        QObject::connect(buttonBox, SIGNAL(accepted()), setSpaceDialog, SLOT(accept()));
        QObject::connect(buttonBox, SIGNAL(rejected()), setSpaceDialog, SLOT(reject()));

        QMetaObject::connectSlotsByName(setSpaceDialog);
    } // setupUi

    void retranslateUi(QDialog *setSpaceDialog)
    {
        setSpaceDialog->setWindowTitle(QApplication::translate("setSpaceDialog", "Dialog", nullptr));
    } // retranslateUi

};

namespace Ui {
    class setSpaceDialog: public Ui_setSpaceDialog {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_SETSPACEDIALOG_H
