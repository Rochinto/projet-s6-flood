
#ifndef _TESTTOOL_H_
#define _TESTTOOL_H_

#define TEST_PREFIX "\33[96m\33[1m[TEST]\33[0m\33[97m "
#define SUCCESS_SUFFIX "\33[92m\33[1m\33[7mpassed\33[0m\33[97m "
#define FAIL_SUFFIX "\33[31m\33[1m\33[7mfailed\33[0m\33[97m "
#define RESET_LINE "\33[0m\n"

#define RET_TEST(bool) if(!(bool))return 1;

void test(int test_result, const char *test_name); //test_result = 0 -> success
void section(const char *section_name);

#endif