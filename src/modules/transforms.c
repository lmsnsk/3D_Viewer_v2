#include "../s21_3D_Viewer.h"

#define MATRIX_SIZE 4
#define PI 3.1415926535897932

void matrix_mul(Data* data, double transform_matrix[MATRIX_SIZE][MATRIX_SIZE]) {
  double vertex[MATRIX_SIZE][1];
  double vertex_res[MATRIX_SIZE][1];

  for (unsigned i = 0; i < data->vertices_count; i++) {
    vertex_res[0][0] = 0;
    vertex_res[1][0] = 0;
    vertex_res[2][0] = 0;
    vertex_res[3][0] = 1;
    vertex[0][0] = data->vertices_arr[i].x;
    vertex[1][0] = data->vertices_arr[i].y;
    vertex[2][0] = data->vertices_arr[i].z;
    vertex[3][0] = 1;

    for (int j = 0; j < MATRIX_SIZE; j++) {
      for (int k = 0; k < MATRIX_SIZE; k++) {
        vertex_res[j][0] += transform_matrix[j][k] * vertex[k][0];
      }
    }
    data->vertices_arr[i].x = vertex_res[0][0];
    data->vertices_arr[i].y = vertex_res[1][0];
    data->vertices_arr[i].z = vertex_res[2][0];
  }
}

void moving(Data* data, double dx, double dy, double dz) {
  double moving_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {1, 0, 0, dx}, {0, 1, 0, dy}, {0, 0, 1, dz}, {0, 0, 0, 1}};
  matrix_mul(data, moving_matrix);
}

void scaling(Data* data, double scale) {
  double scaling_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {scale, 0, 0, 0}, {0, scale, 0, 0}, {0, 0, scale, 0}, {0, 0, 0, 1}};
  matrix_mul(data, scaling_matrix);
}

void rotation_x(Data* data, double alf) {
  alf *= PI / 180;
  double rotation_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {1, 0, 0, 0},
      {0, cos(alf), -sin(alf), 0},
      {0, sin(alf), cos(alf), 0},
      {0, 0, 0, 1}};
  matrix_mul(data, rotation_matrix);
}

void rotation_y(Data* data, double alf) {
  alf *= PI / 180;
  double rotation_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {cos(alf), 0, sin(alf), 0},
      {0, 1, 0, 0},
      {-sin(alf), 0, cos(alf), 0},
      {0, 0, 0, 1}};
  matrix_mul(data, rotation_matrix);
}

void rotation_z(Data* data, double alf) {
  alf *= PI / 180;
  double rotation_matrix[MATRIX_SIZE][MATRIX_SIZE] = {
      {cos(alf), -sin(alf), 0, 0},
      {sin(alf), cos(alf), 0, 0},
      {0, 0, 1, 0},
      {0, 0, 0, 1}};
  matrix_mul(data, rotation_matrix);
}

void auto_scaling(Data* data) {
  double x_max = data->vertices_arr[0].x;
  double y_max = data->vertices_arr[0].y;
  double z_max = data->vertices_arr[0].z;
  double x_min = x_max;
  double y_min = y_max;
  double z_min = z_max;
  for (unsigned i = 1; i < data->vertices_count; i++) {
    if (data->vertices_arr[i].x > x_max) x_max = data->vertices_arr[i].x;
    if (data->vertices_arr[i].x < x_min) x_min = data->vertices_arr[i].x;
    if (data->vertices_arr[i].y > y_max) y_max = data->vertices_arr[i].y;
    if (data->vertices_arr[i].y < y_min) y_min = data->vertices_arr[i].y;
    if (data->vertices_arr[i].z > z_max) z_max = data->vertices_arr[i].z;
    if (data->vertices_arr[i].z < z_min) z_min = data->vertices_arr[i].z;
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

  moving(data, -dx, -dy, -dz);
  scaling(data, scale);
}

void perspective_switch(Data* data, int is_on) {
  double ratio = 0.03;
  if (is_on) {
    for (unsigned i = 0; i < data->vertices_count; i++) {
      data->vertices_arr[i].x *= pow(2, data->vertices_arr[i].z * ratio);
      data->vertices_arr[i].y *= pow(2, data->vertices_arr[i].z * ratio);
    }
  } else {
    for (unsigned i = 0; i < data->vertices_count; i++) {
      data->vertices_arr[i].x /= pow(2, data->vertices_arr[i].z * ratio);
      data->vertices_arr[i].y /= pow(2, data->vertices_arr[i].z * ratio);
    }
  }
}