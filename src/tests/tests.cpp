#include <gtest/gtest.h>

#include "../controller/controller.hpp"
#include "../s21_3D_Viewer.hpp"

TEST(Viewer, read_file) {
  std::string filename = "obj/cube.obj";
  s21::Controller c;
  s21::Data data;
  EXPECT_NO_THROW(c.parseFile(filename, data));
}

TEST(Viewer, invalid_read_file) {
  std::string filename = "obj/nofile";
  s21::Controller c;
  s21::Data data;
  EXPECT_ANY_THROW(c.parseFile(filename, data));
}

TEST(Viewer, empty_file) {
  std::string filename = "obj/Empty.obj";
  s21::Controller c;
  s21::Data data;
  EXPECT_ANY_THROW(c.parseFile(filename, data));
}

TEST(Viewer, invalid_model_1) {
  std::string filename = "obj/error1.obj";
  s21::Controller c;
  s21::Data data;
  EXPECT_ANY_THROW(c.parseFile(filename, data));
}

TEST(Viewer, invalid_model_2) {
  std::string filename = "obj/error.obj";
  s21::Controller c;
  s21::Data data;
  EXPECT_ANY_THROW(c.parseFile(filename, data));
}

TEST(Viewer, transforms_move) {
  std::string filename = "obj/cube.obj";
  s21::Controller c;
  s21::Data data;
  c.parseFile(filename, data);
  c.moveModel(data, s21::DIRECT_X, 2);
  c.moveModel(data, s21::DIRECT_Y, 1);
  c.moveModel(data, s21::DIRECT_Z, 3.5);
  EXPECT_EQ(data.getVertexArray()[0].x, -0.4);
  EXPECT_NO_THROW();
}

TEST(Viewer, transforms_auto_scale) {
  std::string filename = "obj/cube.obj";
  s21::Controller c;
  s21::Data data;
  c.parseFile(filename, data);
  c.autoScaleModel(data);
  EXPECT_NO_THROW();
}

TEST(Viewer, transforms_rotate) {
  std::string filename = "obj/cube.obj";
  s21::Controller c;
  s21::Data data;
  c.parseFile(filename, data);
  c.rotateModel(data, s21::DIRECT_Y, 0.5);
  c.rotateModel(data, s21::DIRECT_X, 1);
  c.rotateModel(data, s21::DIRECT_Z, 45);
  EXPECT_NO_THROW();
}

TEST(Viewer, transforms_scale) {
  std::string filename = "obj/cube.obj";
  s21::Controller c;
  s21::Data data;
  c.parseFile(filename, data);
  c.scaleModel(data, 2);
  EXPECT_NO_THROW();
}

int main(int argc, char **argv) {
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}
