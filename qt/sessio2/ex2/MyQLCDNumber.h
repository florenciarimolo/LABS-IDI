#include <QObject>
#include <QString>
#include <QLCDNumber>

class MyQLCDNumber: public QLCDNumber
{ 
   Q_OBJECT   

   public:
     MyQLCDNumber (QWidget *parent);

   public slots:    
     void mostraNum (int n);
     void reset ();

   signals:         

     
   private:
     QPalette* palette;
};
