
#include "MyQLCDNumber.h"

MyQLCDNumber::MyQLCDNumber(QWidget *parent) : QLCDNumber(parent)
{
    palette = new QPalette();
    palette->setColor(QPalette::WindowText,Qt::green);
    this->setPalette(*palette);
}

void MyQLCDNumber::mostraNum(int n)
{
    if (n == 0) {
        palette->setColor(QPalette::WindowText,Qt::green);
    }
    else if (n%2 != 0){
        palette->setColor(QPalette::WindowText,Qt::red);
    }
    else {
        palette->setColor(QPalette::WindowText,Qt::blue);
    }
    this->setPalette(*palette);
    display(n);
}    

void MyQLCDNumber::reset()
{
    palette->setColor(QPalette::WindowText,Qt::green);
    this->setPalette(*palette);
    display(0);
} 
