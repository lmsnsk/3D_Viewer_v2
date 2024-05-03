#ifndef S21_3D_VIEWER
#define S21_3D_VIEWER

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  double x;
  double y;
  double z;
} Vertex;

typedef struct {
  unsigned* faces;
  unsigned vert_in_face;
} Face;

typedef struct {
  Vertex* vertices_arr;
  Face* faces_arr;
  unsigned vertices_count;
  unsigned faces_count;
} Data;

enum Direct { DIRECT_X, DIRECT_Y, DIRECT_Z };

int parcer(const char* filename, Data* data);

void free_data(Data* data);

void moving(Data* data, double dx, double dy, double dz);

void scaling(Data* data, double scale);

void rotation_x(Data* data, double alf);
void rotation_y(Data* data, double alf);
void rotation_z(Data* data, double alf);

void auto_scaling(Data* data);
void perspective_switch(Data* data, int is_on);

#endif  // S21_3D_VIEWER