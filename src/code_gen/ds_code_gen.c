#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <stdint.h>
#include <errno.h>
#include <string.h>
#include <stdarg.h>
#include "ds_code_gen_common.h"
#include "ds_code_gen_index_gen.h"
#include "ds_code_gen_index_container.h"

char *ds_types[] = {"int8_t", "uint8_t", "int16_t", "uint16_t", "int32_t", "uint32_t", "int64_t", "uint64_t", "float", "double"};

void die(char *str) {
    printf("%s\n", str);
    exit(1);
}

void die_perror(char *str) {
    int BUF_SIZE = 1024;
    char buf[BUF_SIZE + 1];
    snprintf(buf, BUF_SIZE, "%s:%s", strerror(errno), str);
    die(buf);
}

void write_code(FILE *file, const char *format, ...) {
    va_list ap;
    va_start(ap, format);
    if (0 > vfprintf(file, format, ap))
        die("fprintf");
    va_end(ap);
}

int main(int argc, char **argv) {
    if (4 > argc) {
        die("Usage: ds_code BASE_PATH GEN_FILE CONTAINER_FILE");
    }

    char *base_path = argv[1];
    char *gen_file = argv[2];
    char *container_file = argv[3];
    char filename[PATH_MAX];

    if (PATH_MAX <= snprintf(filename, PATH_MAX, "%s/%s", base_path, gen_file))
        die("filename too long");
    ds_code_gen_index_gen(filename);

    if (PATH_MAX <= snprintf(filename, PATH_MAX, "%s/%s", base_path, container_file))
        die("filename too long");
    ds_code_gen_index_container(filename);

    return 0;
}
