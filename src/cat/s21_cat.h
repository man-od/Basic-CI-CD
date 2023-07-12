#ifndef SRC_CAT_S21_CAT_H_
#define SRC_CAT_S21_CAT_H_
#include <errno.h>
#include <getopt.h>
#include <stdio.h>
#include <string.h>
const struct option long_options[] = {
    {"number-nonblank", no_argument, NULL, 'b'},
    {"number", no_argument, NULL, 'n'},
    {"squeeze-blank", no_argument, NULL, 's'}};
void file(char *argv[], int b, int e, int n, int s, int t, int v);
void cat(int b, int e, int n, int s, int t, int v, FILE *fp);
#endif //  SRC_CAT_S21_CAT_H_
