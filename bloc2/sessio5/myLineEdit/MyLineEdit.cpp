
#include "MyLineEdit.h"

MyLineEdit::MyLineEdit(QWidget *parent) : QLineEdit(parent)
{ 
}

void MyLineEdit::tractaReturn()
{
    emit enviaText (text());
}    
