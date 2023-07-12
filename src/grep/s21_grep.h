#ifndef SRC_GREP_S21_GREP_H_
#define SRC_GREP_S21_GREP_H_
#include <getopt.h>
#include <regex.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
  int e;
  int i;
  int v;
  int c;
  int l;
  int n;
  int h;
  int s;
  int f;
  int o;
  int o_abort;
  char *templates;
} flag_opt;

void parcer(int argc, char **argv, flag_opt *opt);
void template_e(flag_opt *opt);
void templates_f(flag_opt *opt);
void output(FILE *fp, flag_opt *opt, int argc, char **argv, int j);
void flag_o(char *str, regex_t reg, regmatch_t *pmatch);
#endif //  SRC_GREP_S21_GREP_H_
