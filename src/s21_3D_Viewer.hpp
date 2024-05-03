#ifndef S21_3D_VIEWER
#define S21_3D_VIEWER

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

namespace s21 {

struct Vertex {
  double x;
  double y;
  double z;
};

struct Face {
  unsigned* faces;
  unsigned vert_in_face;
};

struct Data {
  Vertex* vertices_arr;
  Face* faces_arr;
  unsigned vertices_count;
  unsigned faces_count;
};

enum Direct { DIRECT_X, DIRECT_Y, DIRECT_Z };

class Viewer {
 public:
  Viewer(){};
  ~Viewer(){};

  int parcer(const char* filename, Data* data);

  void free_data(Data* data);

  void moving(Data* data, double dx, double dy, double dz);

  void scaling(Data* data, double scale);

  void rotation_x(Data* data, double alf);
  void rotation_y(Data* data, double alf);
  void rotation_z(Data* data, double alf);

  void auto_scaling(Data* data);
  void perspective_switch(Data* data, int is_on);
};

};  // namespace s21

#endif  // S21_3D_VIEWER