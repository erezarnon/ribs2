#ifndef __DS_CODE_GEN__H_
#define __DS_CODE_GEN__H_

#include "ribs_defs.h"

extern char *ds_types[10];

void die(char *str);
void die_perror(char *str);
void write_code(FILE *file, const char *format, ...);

#endif
