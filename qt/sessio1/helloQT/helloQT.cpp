#include <QApplication>
#include <QPushButton>

int main(int argc, char **argv)
{
  // Instancia d'una aplicacio Qt
  QApplication a(argc, argv);

  // Instancia d'un widget PushButton
  QPushButton hello("Hello Qt !", 0);

  // Inicilaitzacio de la vista
  hello.resize(300, 90);
  hello.show();

  // Pas del control d'execució a l'aplicació
  return a.exec();
}
