#ifndef _CHECKRUNNER_H
#define _CHECKRUNNER_H

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wgnu-zero-variadic-macro-arguments"
#include <check.h>
#pragma clang diagnostic pop

/*
 * Run a check suite. Example:
 *
 *   Suite *foo_suite(void);
 *
 *   conch_check_runsuite(foo_suite());
 *
 * Returns 1 on suite failures, 0 otherwise.
 */
int conch_check_runsuite(Suite *s) {
  int number_failed;
  SRunner *sr;

  sr = srunner_create(s);

  srunner_set_log(sr, "-");
  srunner_run_all(sr, CK_SILENT);
  number_failed = srunner_ntests_failed(sr);

  srunner_free(sr);
  return (number_failed == 0) ? 0 : 1;
}

void add_test_case(Suite *s, char *name, void (*test_case)(int)) {
  TCase *tc = tcase_create(name);
  tcase_add_test(tc, test_case);
  suite_add_tcase(s, tc);
}
#define ADD_TEST_CASE(s, tc) add_test_case(s, #tc, tc)

#define CONCH_CHECK_MAIN(suite)                                                \
  int main(void) { return conch_check_runsuite(suite()); }

#endif /* _CHECKRUNNER_H */
