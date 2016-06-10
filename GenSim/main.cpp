#include "mainwindow.h"
#include <QApplication>
#include <QTranslator>

int main(int argc, char *argv[])
{
  QApplication a(argc, argv);

  QString locale = QLocale::system().name();
  QTranslator translator, systranslator;

  QString syspath = QLibraryInfo::location(QLibraryInfo::TranslationsPath);
  systranslator.load("qt_" + locale, syspath);
  a.installTranslator(&systranslator);

  translator.load(QString("GeneticSim_") + locale, "translations");
  a.installTranslator(&translator);

  MainWindow w;
  w.show();
  return a.exec();
}
