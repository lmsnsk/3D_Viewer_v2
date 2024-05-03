#ifndef GLVIEW_HPP
#define GLVIEW_HPP

#include <QGLWidget>
#include <fstream>

#include "../controller/controller.hpp"

class glView : public QGLWidget {
 public:
  glView();
  QColor lineColor;
  QColor plotColor;
  QColor vertexColor;
  int line_type;
  int line_width;
  int vertex_size;
  int vertex_type;
  int projection_type;

  void initializeGL() override;
  void resizeGL(int w, int h) override;
  void paintGL() override;
  void drawScene(s21::Data &myDat);
};

#endif  // GLVIEW_HPP
