#include "controller.hpp"

void parse_file(std::string filename, s21::Data &data) {
  s21::Parcer p;
  p.parcer(data, filename);
};

void auto_scale_model(s21::Data &data) {
  s21::Transformer t;
  t.auto_scaling(data);
};

void scale_model(s21::Data &data, double scale) {
  s21::Transformer t;
  t.scaling(data, scale);
};

void move_model(s21::Data &data, s21::Direct direct, double step) {
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

void rotate_model(s21::Data &data, s21::Direct direct, double step) {
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
