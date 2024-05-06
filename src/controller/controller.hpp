#ifndef S21_CONTROLLER_HPP
#define S21_CONTROLLER_HPP

#include "../s21_3D_Viewer.hpp"

namespace s21 {

struct Controller {
  void parseFile(const std::string &filename, s21::Data &data);
  void autoScaleModel(s21::Data &data);
  void scaleModel(s21::Data &data, double scale);
  void moveModel(s21::Data &data, s21::Direct direct, double step);
  void rotateModel(s21::Data &data, s21::Direct direct, double step);
};

}  // namespace s21

#endif  // S21_CONTROLLER_HPP