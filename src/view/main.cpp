#include <QApplication>

#include "mainwindow.h"

// #include "glview.hpp"

int main(int argc, char *argv[]) {
  QApplication a(argc, argv);
  setlocale(LC_NUMERIC, "C");
  //    glView win;
  //    win.show();

  MainWindow w;
  w.show();

  return a.exec();
}
