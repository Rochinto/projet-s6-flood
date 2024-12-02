
#include "testool.h"
#include <stdio.h>

void test(int test_result, const char *test_name){
    printf(TEST_PREFIX);
    if (test_result) printf(FAIL_SUFFIX);
    else printf(SUCCESS_SUFFIX);
    printf("%s", test_name);
    printf(RESET_LINE);
}

void section(const char *section_name){
    printf(TEST_PREFIX);
    printf("Section : %s", section_name);
    printf(RESET_LINE);
}