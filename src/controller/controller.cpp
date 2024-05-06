#include "controller.hpp"

namespace s21 {

void Controller::parseFile(const std::string &filename, s21::Data &data) {
  s21::Parser p;
  p.parser(data, filename);
};

void Controller::autoScaleModel(s21::Data &data) {
  s21::Transformer t;
  t.auto_scaling(data);
};

void Controller::scaleModel(s21::Data &data, double scale) {
  s21::Transformer t;
  t.scaling(data, scale);
};

void Controller::moveModel(s21::Data &data, s21::Direct direct, double step) {
  s21::Transformer t;
  switch (direct) {
    case s21::DIRECT_X:
      t.moving(data, step, 0, 0);
      break;
    case s21::DIRECT_Y:
      t.moving(data, 0, step, 0);
      break;
    case s21::DIRECT_Z:
      t.moving(data, 0, 0, step);
      break;
  };
};

void Controller::rotateModel(s21::Data &data, s21::Direct direct, double step) {
  s21::Transformer t;
  switch (direct) {
    case s21::DIRECT_X:
      t.rotation_x(data, step);
      break;
    case s21::DIRECT_Y:
      t.rotation_y(data, step);
      break;
    case s21::DIRECT_Z:
      t.rotation_z(data, step);
      break;
  };
};

}  // namespace s21