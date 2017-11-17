/********************************************************************************
** Form generated from reading UI file 'myForm.ui'
**
** Created by: Qt User Interface Compiler version 5.6.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYFORM_H
#define UI_MYFORM_H

#include <MyLineEdit.h>
#include <MyQLabel.h>
#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_Form
{
public:
    QWidget *widget;
    QHBoxLayout *horizontalLayout;
    QSlider *horizontalSlider;
    QPushButton *pushButton;
    QWidget *widget1;
    QHBoxLayout *horizontalLayout_2;
    QLabel *label_2;
    MyQLabel *label_3;
    QWidget *widget2;
    QHBoxLayout *horizontalLayout_3;
    QLabel *label;
    MyLineEdit *lineEdit;

    void setupUi(QWidget *Form)
    {
        if (Form->objectName().isEmpty())
            Form->setObjectName(QStringLiteral("Form"));
        Form->resize(400, 237);
        widget = new QWidget(Form);
        widget->setObjectName(QStringLiteral("widget"));
        widget->setGeometry(QRect(40, 160, 341, 35));
        horizontalLayout = new QHBoxLayout(widget);
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        horizontalLayout->setContentsMargins(0, 0, 0, 0);
        horizontalSlider = new QSlider(widget);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setOrientation(Qt::Horizontal);

        horizontalLayout->addWidget(horizontalSlider);

        pushButton = new QPushButton(widget);
        pushButton->setObjectName(QStringLiteral("pushButton"));

        horizontalLayout->addWidget(pushButton);

        widget1 = new QWidget(Form);
        widget1->setObjectName(QStringLiteral("widget1"));
        widget1->setGeometry(QRect(40, 110, 341, 19));
        horizontalLayout_2 = new QHBoxLayout(widget1);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout_2->setContentsMargins(0, 0, 0, 0);
        label_2 = new QLabel(widget1);
        label_2->setObjectName(QStringLiteral("label_2"));

        horizontalLayout_2->addWidget(label_2);

        label_3 = new MyQLabel(widget1);
        label_3->setObjectName(QStringLiteral("label_3"));

        horizontalLayout_2->addWidget(label_3);

        widget2 = new QWidget(Form);
        widget2->setObjectName(QStringLiteral("widget2"));
        widget2->setGeometry(QRect(40, 40, 341, 33));
        horizontalLayout_3 = new QHBoxLayout(widget2);
        horizontalLayout_3->setObjectName(QStringLiteral("horizontalLayout_3"));
        horizontalLayout_3->setContentsMargins(0, 0, 0, 0);
        label = new QLabel(widget2);
        label->setObjectName(QStringLiteral("label"));

        horizontalLayout_3->addWidget(label);

        lineEdit = new MyLineEdit(widget2);
        lineEdit->setObjectName(QStringLiteral("lineEdit"));

        horizontalLayout_3->addWidget(lineEdit);


        retranslateUi(Form);
        QObject::connect(pushButton, SIGNAL(clicked(bool)), pushButton, SLOT(close()));
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), label_3, SLOT(fixaN(int)));
        QObject::connect(lineEdit, SIGNAL(enviaText(QString)), label_3, SLOT(setText(QString)));
        QObject::connect(lineEdit, SIGNAL(textChanged(QString)), lineEdit, SLOT(setText(QString)));

        QMetaObject::connectSlotsByName(Form);
    } // setupUi

    void retranslateUi(QWidget *Form)
    {
        Form->setWindowTitle(QApplication::translate("Form", "Form", 0));
        pushButton->setText(QApplication::translate("Form", "Sortir", 0));
        label_2->setText(QApplication::translate("Form", "Text retallat:", 0));
        label_3->setText(QString());
        label->setText(QApplication::translate("Form", "Text:", 0));
    } // retranslateUi

};

namespace Ui {
    class Form: public Ui_Form {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYFORM_H
