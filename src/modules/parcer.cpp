#include "../s21_3D_Viewer.hpp"

namespace s21 {

std::vector<Vertex>& Data::getVertexArray() { return vertices_arr_; };

std::vector<Face>& Data::getFaceArray() { return faces_arr_; };

void Parcer::skipSpaces(std::string& str) {
  unsigned i{};
  for (; str[i] != ' '; ++i) {
    if (str[i] == '\n' || str[i] == 0) break;
  }
  while (str[i] == ' ') ++i;
  str = str.substr(i);
}

void Parcer::getVertices(Data& data, std::string& buffer) {
  skipSpaces(buffer);
  Vertex point;

  int scan =
      sscanf(buffer.c_str(), " %lf %lf %lf", &point.x, &point.y, &point.z);

  if (scan >= 3)
    data.getVertexArray().push_back(point);
  else
    throw "Error";
}

void Parcer::getFaces(Data& data, std::string& buffer) {
  skipSpaces(buffer);

  int scan_vert{};
  Face face;

  while (buffer[0] != '\n' && buffer[0] != '\r' && buffer[0] != 0) {
    unsigned vertex_read;
    scan_vert = sscanf(buffer.c_str(), "%u", &vertex_read);
    if (scan_vert) face.vertices.push_back(vertex_read);
    skipSpaces(buffer);
  }
  face.vert_in_face = face.vertices.size();

  if (face.vert_in_face >= 3)
    data.getFaceArray().push_back(face);
  else
    throw "Error";
}

void Parcer::getValues(Data& data, std::string& buffer) {
  if (buffer[0] == 'v' && buffer[1] == ' ') {
    getVertices(data, buffer);
  } else if (buffer[0] == 'f' && buffer[1] == ' ') {
    getFaces(data, buffer);
  }
}

void Parcer::parcer(Data& data, std::string filename) {
  std::ifstream file(filename);
  if (!file.is_open()) {
    throw "Error";
  }
  std::string buffer;
  while (std::getline(file, buffer)) {
    getValues(data, buffer);
    buffer.clear();
  }
  if (data.getFaceArray().empty() || data.getVertexArray().size() < 3)
    throw "Error";
  file.close();
}

}  // namespace s21

// int main() {
//   try {
//     s21::Data myData;
//     s21::Parcer parcer;
//     parcer.parcer(myData, "../obj/error1.obj");

//     for (auto elem : myData.getVertexArray()) {
//       std::cout << "v " << elem.x << " " << elem.y << " " << elem.z
//                 << std::endl;
//     }

//     for (auto elem : myData.getFaceArray()) {
//       std::cout << "f ";
//       for (auto e : elem.vertices) {
//         std::cout << e << " ";
//       }
//       std::cout << std::endl;
//     }

//   } catch (...) {
//     std::cout << "Error" << std::endl;
//   }
//   return 0;
// }
