
#include "MyLineEdit.h"

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent)
{ 
}

void MyLineEdit::setText()
{
    emit enviaText (text());
}    
