#include <iostream>

#include "../s21_3D_Viewer.hpp"

namespace s21 {

const double PI = 3.1415926535897932;

void Transformer::matrix_mul(
    Data& data, double transform_matrix[MATRIX_SIZE][MATRIX_SIZE]) {
  double vertex[MATRIX_SIZE][1];
  double vertex_res[MATRIX_SIZE][1];

  for (unsigned i = 0; i < data.getVertexArray().size(); i++) {
    vertex_res[0][0] = 0;
    vertex_res[1][0] = 0;
    vertex_res[2][0] = 0;
    vertex_res[3][0] = 1;
    vertex[0][0] = data.getVertexArray()[i].x;
    vertex[1][0] = data.getVertexArray()[i].y;
    vertex[2][0] = data.getVertexArray()[i].z;
    vertex[3][0] = 1;

    for (int j = 0; j < MATRIX_SIZE; j++) {
      for (int k = 0; k < MATRIX_SIZE; k++) {
        vertex_res[j][0] += transform_matrix[j][k] * vertex[k][0];
      }
    }
    data.getVertexArray()[i].x = vertex_res[0][0];
    data.getVertexArray()[i].y = vertex_res[1][0];
    data.getVertexArray()[i].z = vertex_res[2][0];
  }
}

void Transformer::moving(Data& data, double dx, double dy, double dz) {
  double moving_matrix[MATRIX_SIZE][MATRIX_SIZE] = {{1, 0, 0, dx * factor},
                                                    {0, 1, 0, dy * factor},
                                                    {0, 0, 1, dz * factor},
                                                    {0, 0, 0, 1}};
  matrix_mul(data, moving_matrix);
}

void Transformer::scaling(Data& data, double scale) {
  double scaling_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {scale, 0, 0, 0}, {0, scale, 0, 0}, {0, 0, scale, 0}, {0, 0, 0, 1}};
  matrix_mul(data, scaling_matrix);
}

void Transformer::rotation_x(Data& data, double alf) {
  alf *= PI / 180;
  double rotation_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {1, 0, 0, 0},
      {0, cos(alf), -sin(alf), 0},
      {0, sin(alf), cos(alf), 0},
      {0, 0, 0, 1}};
  matrix_mul(data, rotation_matrix);
}

void Transformer::rotation_y(Data& data, double alf) {
  alf *= PI / 180;
  double rotation_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {cos(alf), 0, sin(alf), 0},
      {0, 1, 0, 0},
      {-sin(alf), 0, cos(alf), 0},
      {0, 0, 0, 1}};
  matrix_mul(data, rotation_matrix);
}

void Transformer::rotation_z(Data& data, double alf) {
  alf *= PI / 180;
  double rotation_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {cos(alf), -sin(alf), 0, 0},
      {sin(alf), cos(alf), 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}};
  matrix_mul(data, rotation_matrix);
}

void Transformer::auto_scaling(Data& data) {
  double x_max = data.getVertexArray()[0].x;
  double y_max = data.getVertexArray()[0].y;
  double z_max = data.getVertexArray()[0].z;
  double x_min = x_max;
  double y_min = y_max;
  double z_min = z_max;
  for (unsigned i = 1; i < data.getVertexArray().size(); i++) {
    if (data.getVertexArray()[i].x > x_max) x_max = data.getVertexArray()[i].x;
    if (data.getVertexArray()[i].x < x_min) x_min = data.getVertexArray()[i].x;
    if (data.getVertexArray()[i].y > y_max) y_max = data.getVertexArray()[i].y;
    if (data.getVertexArray()[i].y < y_min) y_min = data.getVertexArray()[i].y;
    if (data.getVertexArray()[i].z > z_max) z_max = data.getVertexArray()[i].z;
    if (data.getVertexArray()[i].z < z_min) z_min = data.getVertexArray()[i].z;
  }
  double range_x = 10 / (x_max - x_min);
  double range_y = 10 / (y_max - y_min);
  double range_z = 10 / (z_max - z_min);
  double dx = (x_max - x_min) / 2 + x_min;
  double dy = (y_max - y_min) / 2 + y_min;
  double dz = (z_max - z_min) / 2 + z_min;
  double scale = (range_x > range_y)
                     ? ((range_z > range_y) ? range_y : range_z)
                     : ((range_z > range_x) ? range_x : range_z);
  moving(data, -dx / factor, -dy / factor, -dz / factor);
  scaling(data, scale);
}

}  // namespace s21