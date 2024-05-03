#ifndef S21_CONTROLLER_HPP
#define S21_CONTROLLER_HPP

#include "../s21_3D_Viewer.hpp"

void parse_file(std::string filename, s21::Data &data);
void auto_scale_model(s21::Data &data);
void scale_model(s21::Data &data, double scale);
void move_model(s21::Data &data, s21::Direct direct, double step);
void rotate_model(s21::Data &data, s21::Direct direct, double step);

#endif  // S21_CONTROLLER_HPP