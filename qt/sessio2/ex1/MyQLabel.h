#include <QObject>
#include <QString>
#include <QLabel>

class MyQLabel: public QLabel
{ 
   Q_OBJECT   

   public:
     MyQLabel (QWidget *parent);

   public slots:    
     void truncaText (QString text); 
     void fixaN (int n);

   signals:         

     
   private:
     QString text;
     int n;
};
