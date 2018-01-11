#include <QObject>
#include <QString>
#include <QLabel>
#include <string>

class MyLabel: public QLabel
{ 
   Q_OBJECT   

   public:
     MyLabel (QWidget *parent);

   public slots:    
     void pintaR (int value);
     void pintaG (int value);
     void pintaB (int value);

    private:
    int r, g, b;
    
    void canviaColor();
};
