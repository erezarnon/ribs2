#include "minunit.h"
#include "test_kmeans.h"
#include "test_ds_var_field.h"
#include "test_zlib.h"

static const char *all_tests() {
    mu_run_test(test_kmeans);
    mu_run_test(test_ds_var_field);
    mu_run_test(test_zlib_vmbuf);
    return 0;
}

int main() {
    return mu_run_tests_wrapper(all_tests);
}