
#include "MyQLabel.h"

MyQLabel::MyQLabel(QWidget *parent) : QLabel(parent)
{
    n = 0;
    text = "";
}

void MyQLabel::truncaText(QString nouText)
{
    text = nouText;
    nouText.truncate(n);
    setText (nouText);
}    

void MyQLabel::fixaN(int nChars)
{
    n = nChars;
    QString t = text;
    t.truncate(n);
    setText(t);
} 
