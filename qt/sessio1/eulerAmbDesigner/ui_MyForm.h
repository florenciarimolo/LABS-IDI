/********************************************************************************
** Form generated from reading UI file 'MyForm.ui'
**
** Created by: Qt User Interface Compiler version 5.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_MYFORM_H
#define UI_MYFORM_H

#include <QtCore/QVariant>
#include <QtWidgets/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QButtonGroup>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QHeaderView>
#include <QtWidgets/QLabel>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QSlider>
#include <QtWidgets/QSpacerItem>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>
#include "MyEuler.h"

QT_BEGIN_NAMESPACE

class Ui_MyForm
{
public:
    QHBoxLayout *horizontalLayout_2;
    QHBoxLayout *horizontalLayout;
    MyEuler *myForm;
    QSpacerItem *horizontalSpacer;
    QVBoxLayout *verticalLayout;
    QLabel *label;
    QSlider *horizontalSlider;
    QLabel *label_2;
    QSlider *horizontalSlider_2;
    QSpacerItem *verticalSpacer;
    QPushButton *pushButton;

    void setupUi(QWidget *MyForm)
    {
        if (MyForm->objectName().isEmpty())
            MyForm->setObjectName(QStringLiteral("MyForm"));
        MyForm->resize(929, 733);
        horizontalLayout_2 = new QHBoxLayout(MyForm);
        horizontalLayout_2->setObjectName(QStringLiteral("horizontalLayout_2"));
        horizontalLayout = new QHBoxLayout();
        horizontalLayout->setObjectName(QStringLiteral("horizontalLayout"));
        myForm = new MyEuler(MyForm);
        myForm->setObjectName(QStringLiteral("myForm"));
        myForm->setMinimumSize(QSize(691, 668));

        horizontalLayout->addWidget(myForm);

        horizontalSpacer = new QSpacerItem(48, 17, QSizePolicy::Expanding, QSizePolicy::Minimum);

        horizontalLayout->addItem(horizontalSpacer);


        horizontalLayout_2->addLayout(horizontalLayout);

        verticalLayout = new QVBoxLayout();
        verticalLayout->setObjectName(QStringLiteral("verticalLayout"));
        label = new QLabel(MyForm);
        label->setObjectName(QStringLiteral("label"));

        verticalLayout->addWidget(label);

        horizontalSlider = new QSlider(MyForm);
        horizontalSlider->setObjectName(QStringLiteral("horizontalSlider"));
        horizontalSlider->setMinimumSize(QSize(160, 37));
        horizontalSlider->setMinimum(-360);
        horizontalSlider->setMaximum(360);
        horizontalSlider->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalSlider);

        label_2 = new QLabel(MyForm);
        label_2->setObjectName(QStringLiteral("label_2"));

        verticalLayout->addWidget(label_2);

        horizontalSlider_2 = new QSlider(MyForm);
        horizontalSlider_2->setObjectName(QStringLiteral("horizontalSlider_2"));
        horizontalSlider_2->setMinimum(-360);
        horizontalSlider_2->setMaximum(360);
        horizontalSlider_2->setOrientation(Qt::Horizontal);

        verticalLayout->addWidget(horizontalSlider_2);

        verticalSpacer = new QSpacerItem(20, 348, QSizePolicy::Minimum, QSizePolicy::Expanding);

        verticalLayout->addItem(verticalSpacer);

        pushButton = new QPushButton(MyForm);
        pushButton->setObjectName(QStringLiteral("pushButton"));
        QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
        sizePolicy.setHorizontalStretch(0);
        sizePolicy.setVerticalStretch(0);
        sizePolicy.setHeightForWidth(pushButton->sizePolicy().hasHeightForWidth());
        pushButton->setSizePolicy(sizePolicy);
        pushButton->setStyleSheet(QStringLiteral("background-color:red"));

        verticalLayout->addWidget(pushButton);


        horizontalLayout_2->addLayout(verticalLayout);


        retranslateUi(MyForm);
        QObject::connect(pushButton, SIGNAL(clicked()), MyForm, SLOT(close()));
        QObject::connect(horizontalSlider, SIGNAL(valueChanged(int)), myForm, SLOT(psiAngle(int)));
        QObject::connect(horizontalSlider_2, SIGNAL(valueChanged(int)), myForm, SLOT(thetaAngle(int)));

        QMetaObject::connectSlotsByName(MyForm);
    } // setupUi

    void retranslateUi(QWidget *MyForm)
    {
        MyForm->setWindowTitle(QApplication::translate("MyForm", "Form", Q_NULLPTR));
        label->setText(QApplication::translate("MyForm", "Gir eix Y", Q_NULLPTR));
        label_2->setText(QApplication::translate("MyForm", "Gir eix X", Q_NULLPTR));
        pushButton->setText(QApplication::translate("MyForm", "&Sortir", Q_NULLPTR));
    } // retranslateUi

};

namespace Ui {
    class MyForm: public Ui_MyForm {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_MYFORM_H
