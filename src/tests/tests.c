#include <check.h>

#include "../s21_3D_Viewer.h"

START_TEST(read_file) {
  char filename[] = "obj/cube.obj";
  Data data;
  int error = parcer(filename, &data);
  free_data(&data);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(invalid_read_file) {
  char filename[] = "../obj/nofile";
  Data data;
  int error = parcer(filename, &data);
  ck_assert_int_eq(error, 1);
}
END_TEST

START_TEST(transforms_1) {
  char filename[] = "obj/cube.obj";
  Data data;
  int error = parcer(filename, &data);
  moving(&data, 3.4, 12, -66.12345);
  scaling(&data, 2);
  rotation_x(&data, 30);
  rotation_y(&data, -30);
  rotation_z(&data, 55.4);
  free_data(&data);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(transforms_2) {
  char filename[] = "obj/cube.obj";
  Data data;
  int error = parcer(filename, &data);
  auto_scaling(&data);
  free_data(&data);
  ck_assert_int_eq(error, 0);
}
END_TEST

START_TEST(transforms_3) {
  char filename[] = "obj/cube.obj";
  Data data;
  int error = parcer(filename, &data);
  perspective_switch(&data, 1);
  perspective_switch(&data, 0);
  free_data(&data);
  ck_assert_int_eq(error, 0);
}
END_TEST
Suite *suite_viewer(void) {
  Suite *s = suite_create("suite_viewer");
  TCase *tc = tcase_create("case_viewer");

  tcase_add_test(tc, read_file);
  tcase_add_test(tc, invalid_read_file);
  tcase_add_test(tc, transforms_1);
  tcase_add_test(tc, transforms_2);
  tcase_add_test(tc, transforms_3);

  suite_add_tcase(s, tc);
  return s;
}

void run_testcase(Suite *testcase) {
  printf("\n");
  SRunner *sr = srunner_create(testcase);

  srunner_set_fork_status(sr, CK_NOFORK);
  srunner_run_all(sr, CK_NORMAL);

  srunner_free(sr);
}

int main(void) {
  printf("===========================================\n");
  run_testcase(suite_viewer());
  printf("\n===========================================\n\n");

  return 0;
}
