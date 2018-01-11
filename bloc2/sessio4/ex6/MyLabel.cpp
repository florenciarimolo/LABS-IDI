
#include "MyLabel.h"

MyLabel::MyLabel(QWidget *parent) : QLabel(parent)
{
    r = g = b = 0;
    canviaColor();
}

void MyLabel::pintaR (int value)
{
    r = value;
    canviaColor();
}    

void MyLabel::pintaG (int value)
{
    g = value;
    canviaColor();
}

void MyLabel::pintaB (int value)
{
    b = value;
    canviaColor();
}

void MyLabel::canviaColor()
{
    QString style = QString("QLabel {background-color: rgb("+QString::number(r)+','+QString::number(g)+','+QString::number(b)+") }");
    this->setStyleSheet(style);
}
