
#include "MyQLabel.h"

MyQLabel::MyQLabel(QWidget *parent) : MyQLabel(parent)
{
    n = 0;
}

void MyQLabel::truncaText(const QString text)
{
    QString leftSide = text.left(n);
    setText (leftSide());
}    

void MyQLabel::fixaN(int llargada)
{
    n = llargada;
} 
