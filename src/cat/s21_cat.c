#include "s21_cat.h"
int main(int argc, char *argv[]) {
  int option = 0, b = 0, e = 0, n = 0, s = 0, t = 0, v = 0, exit = 0;
  const char *short_options = "beEstTnv";
  while ((option = getopt_long(argc, argv, short_options, long_options,
                               NULL)) != -1) {
    switch (option) {
    case 'b':
      b = 1;
      break;
    case 'e':
      e = 1;
      v = 1;
      break;
    case 'E':
      e = 1;
      break;
    case 'n':
      n = 1;
      break;
    case 's':
      s = 1;
      break;
    case 't':
      t = 1;
      v = 1;
      break;
    case 'T':
      t = 1;
      break;
    case 'v':
      v = 1;
      break;
    default:
      fprintf(stderr, "cat: illegal option -- %c\n", option);
      fprintf(stderr, "usage: cat [-benstuv] [file ...]\n");
      exit = 1;
      break;
    }
  }
  if (exit == 0) {
    while (optind < argc) {
      file(argv, b, e, n, s, t, v);
      optind++;
    }
  }
  return 0;
}
void file(char *argv[], int b, int e, int n, int s, int t, int v) {
  FILE *fp = NULL;
  if (b && n)
    n = 0;
  fp = fopen(argv[optind], "r");
  if (fp == 0) {
    fprintf(stderr, "s21_cat: %s: %s\n", argv[optind], strerror(errno));
  } else {
    cat(b, e, n, s, t, v, fp);
    fclose(fp);
  }
}
void cat(int b, int e, int n, int s, int t, int v, FILE *fp) {
  int current;
  int prev = -1;
  int temp = 0;
  int count = 1, first = 1;
  while ((current = fgetc(fp)) != EOF) {
    if (s) {
      if (current == '\n')
        temp++;
      if (current != '\n')
        temp = 0;
    }
    if (current == '\n' && (!s || temp < 3)) {
      if (n && (prev == '\n' || prev == -1))
        printf("%6d\t", count++);
      if (e) {
        printf("$");
      }
      printf("%c", current);
    }
    if (current != '\n') {
      if ((prev == '\n' || first == 1) && (n || b)) {
        printf("%6d\t", count++);
      }
      if (current < 32 && current != 9 && current != 10 && v) {
        printf("^%c", current + 64);
      } else if (current > 127 && current < 160 && v) {
        printf("^%c", current - 64);
      } else if (current == 127 && v) {
        printf("^%c", current - 64);
      } else if (current == '\t' && t) {
        printf("^I");
      } else
        printf("%c", current);
    }
    prev = current;
    first = 0;
  }
}
