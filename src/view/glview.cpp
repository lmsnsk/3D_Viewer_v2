#include "glview.hpp"

#include <GL/gl.h>
#include <GL/glu.h>

#include <QDebug>
#include <iostream>

glView::glView() {}

void glView::initializeGL() {
  glEnable(GL_DEPTH_TEST);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  GLfloat aspectRatio = (GLfloat)width() / (GLfloat)height();
  qglClearColor(plotColor);
  glClear(GL_COLOR_BUFFER_BIT);

  if (projection_type == 0) {
    if (width() <= height())
      glOrtho(-10.0, 10.0, -10 / aspectRatio, 10.0 / aspectRatio, -10.0, 100.0);
    else
      glOrtho(-10.0 * aspectRatio, 10.0 * aspectRatio, -10.0, 10.0, -10.0,
              100.0);
  } else {
    gluPerspective(45.0, aspectRatio, 1.0, 100.0);
  }
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(0.0, 0.0, 10.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void glView::resizeGL(int w, int h) { glViewport(0, 0, w, h); }

void glView::drawScene(s21::Data &myData) {
  qglClearColor(plotColor);
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  qglColor(lineColor);
  glLineWidth(line_width);
  if (line_type == 1) {
    glEnable(GL_LINE_STIPPLE);
    glLineStipple(1, 0x00F0);
  }
  glBegin(GL_LINES);

  for (unsigned int i = 0; i < myData.getFaceArray().size(); ++i) {
    if (!myData.getFaceArray()[i].vertices.empty()) {
      for (unsigned int j = 0; j < myData.getFaceArray()[i].vert_in_face; ++j) {
        int vertexIndex1 = myData.getFaceArray()[i].vertices[j];
        int vertexIndex2 =
            myData.getFaceArray()[i]
                .vertices[(j + 1) % myData.getFaceArray()[i].vert_in_face];
        glVertex3f(myData.getVertexArray()[vertexIndex1 - 1].x,
                   myData.getVertexArray()[vertexIndex1 - 1].y,
                   myData.getVertexArray()[vertexIndex1 - 1].z);
        glVertex3f(myData.getVertexArray()[vertexIndex2 - 1].x,
                   myData.getVertexArray()[vertexIndex2 - 1].y,
                   myData.getVertexArray()[vertexIndex2 - 1].z);
      }
    }
  }
  glEnd();
  if (line_type == 1) glDisable(GL_LINE_STIPPLE);

  if (vertex_type == 1 || vertex_type == 2) {
    glPointSize(vertex_size);
    if (vertex_type == 1) glEnable(GL_POINT_SMOOTH);
    glBegin(GL_POINTS);
    qglColor(vertexColor);

    for (unsigned i = 0; i < myData.getVertexArray().size(); i++) {
      if (!myData.getVertexArray().empty()) {
        glVertex3f(myData.getVertexArray()[i].x, myData.getVertexArray()[i].y,
                   myData.getVertexArray()[i].z);
      }
    }

    glEnd();
    if (vertex_type == 1) glDisable(GL_POINT_SMOOTH);
  }
}

void glView::paintGL() {}
