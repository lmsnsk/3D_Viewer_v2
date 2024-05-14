#ifndef S21_3D_VIEWER
#define S21_3D_VIEWER

#include <cmath>
#include <fstream>
#include <string>
#include <vector>

namespace s21 {

const int MATRIX_SIZE = 4;

struct Vertex {
  double x;
  double y;
  double z;
};

struct Face {
  std::vector<unsigned> vertices;
  unsigned vert_in_face{};
};

enum Direct { DIRECT_X, DIRECT_Y, DIRECT_Z };

class Data {
 public:
  std::vector<Vertex>& getVertexArray();
  std::vector<Face>& getFaceArray();
  void clearData();

 private:
  std::vector<Vertex> vertices_arr_;
  std::vector<Face> faces_arr_;
};

class Parser {
 public:
  void parser(Data& data, std::string filename);

 private:
  void getValues(Data& data, std::string& buffer);
  void getFaces(Data& data, std::string& buffer);
  void skipSpaces(std::string& str);
  void getVertices(Data& data, std::string& buffer);
};

class Transformer {
 private:
  double transform_matrix[MATRIX_SIZE][MATRIX_SIZE];
  double factor{0.05};

 public:
  Transformer() : transform_matrix(){};
  ~Transformer(){};

  void moving(Data& data, double dx, double dy, double dz);

  void scaling(Data& data, double scale);

  void rotation_x(Data& data, double alf);
  void rotation_y(Data& data, double alf);
  void rotation_z(Data& data, double alf);

  void auto_scaling(Data& data);

 private:
  void matrix_mul(Data& data,
                  double transform_matrix[MATRIX_SIZE][MATRIX_SIZE]);
};

}  // namespace s21

#endif  // S21_3D_VIEWER