
#include <QLabel>

class MyQLabel: public QLabel
{ 
   Q_OBJECT   

   public:
     MyQLabel (QWidget *parent);

   public slots:    
     void truncaText (const QString text); 
     void fixaN (int n);

   signals:         

     
   private:
     int n;
};
