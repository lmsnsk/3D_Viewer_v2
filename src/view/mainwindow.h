#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QButtonGroup>
#include <QMainWindow>
#include <fstream>

#include "glview.hpp"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;

}
QT_END_NAMESPACE

class MainWindow : public QMainWindow {
  Q_OBJECT

 public:
  MainWindow(QWidget *parent = nullptr);
  ~MainWindow();
  QColor lineColor;
  QColor plotColor;
  QColor vertexColor;
  int line_type;
  int line_width;
  int vertex_size;
  int vertex_type;
  int projection_type;

 public slots:
  void move_foo();
  void rotate_foo();
  void scale_foo();
  void saveSettings();

 private slots:
  void on_openFile_clicked();
  void directionChanged(int id);
  void on_linesColorBtn_clicked();
  void on_plotColorBtn_clicked();
  void on_linesType_activated(int index);
  void on_linesWidthAccept_clicked();
  void on_vertexType_activated(int index);
  void on_vertexColorBtn_clicked();
  void on_vertexSizeAccept_clicked();
  void on_perspective_stateChanged(int arg1);
  void on_getScreenBMP_clicked();
  void on_getScreenGIF_clicked();
  void on_getScreenJPG_clicked();

 private:
  Ui::MainWindow *ui;
  glView mView;
  Data myData{};
  QButtonGroup *directionButtonGroup;
  Direct direct;
  bool is_model_opened{false};
  int bmp_counter{};
  int jpg_counter{};
  int gif_counter{};
  void writeSettings(std::ofstream &outFile);
  void setSettings();
  void readSettings(std::ifstream &outFile);
  // void save_image(int &counter, std::string dot_format, std::string format);

  void captureFramesAndCreateGIF(const QString &outputFileName, int durationMs,
                                 int numFrames);
  void captureFrame(const QString &outputFilePath);
  void createGIF(const QString &inputDir, const QString &outputFileName,
                 int durationMs);
};
#endif  // MAINWINDOW_H
