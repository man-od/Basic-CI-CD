#include "s21_grep.h"
int main(int argc, char **argv) {
  flag_opt opt = {0};
  FILE *in;
  if (argc > 2) {
    opt.templates = malloc(2 * sizeof(char));
    opt.templates[0] = '\0';
    parcer(argc, argv, &opt);
    for (int i = optind; i < argc; i++) {
      if ((in = fopen(argv[i], "r")) == NULL) {
        if (!opt.s) {
          fprintf(stderr, "grep: %s: No such file or directory\n", argv[i]);
        }
      } else {
        output(in, &opt, argc, argv, i);
        fclose(in);
      }
    }
    free(opt.templates);
  }
  return 0;
}

void parcer(int argc, char **argv, flag_opt *opt) {
  int flag;
  while ((flag = getopt_long(argc, argv, "e:ivclnhsf:o", NULL, NULL)) != -1) {
    switch (flag) {
    case 'e':
      template_e(opt);
      break;
    case 'i':
      opt->i = 1;
      break;
    case 'v':
      opt->v = 1;
      break;
    case 'c':
      opt->c = 1;
      break;
    case 'l':
      opt->l = 1;
      break;
    case 'n':
      opt->n = 1;
      break;
    case 'h':
      opt->h = 1;
      break;
    case 's':
      opt->s = 1;
      break;
    case 'f':
      templates_f(opt);
      break;
    case 'o':
      opt->o = 1;
      break;
    default:
      if (opt->templates) {
        free(opt->templates);
      }
      exit(1);
      break;
    }
  }
  if (!(opt->e)) {
    if (strcmp(argv[optind], "") == 0) {
      opt->o_abort = 1;
      opt->templates = strcat(opt->templates, ".");
      optind++;
    } else {
      opt->templates = realloc(opt->templates, strlen(argv[optind]) + 1);
      opt->templates = strcat(opt->templates, argv[optind++]);
    }
  } else if (opt->templates[strlen(opt->templates) - 1] == '|') {
    opt->templates[strlen(opt->templates) - 1] = '\0';
  }
}

void template_e(flag_opt *opt) {
  opt->e = 1;
  if (strcmp(optarg, "") == 0) {
    opt->o_abort = 1;
    opt->templates = realloc(opt->templates, strlen(opt->templates) + 3);
    opt->templates = strcat(opt->templates, ".");
  } else {
    opt->templates =
        realloc(opt->templates, strlen(opt->templates) + strlen(optarg) + 2);
    opt->templates = strcat(opt->templates, optarg);
  }
  opt->templates = strcat(opt->templates, "|");
}

void templates_f(flag_opt *opt) {
  opt->e = 1;
  if (opt->f) {
    opt->templates = realloc(opt->templates, strlen(opt->templates) + 2);
    opt->templates = strcat(opt->templates, "|");
  }
  opt->f = 1;
  char ch, prev;
  FILE *fin = NULL;
  if ((fin = fopen(optarg, "r")) != NULL) {
    for (prev = '|'; (ch = getc(fin)) != EOF; prev = ch) {
      if (ch == '\n') {
        ch = '|';
        if (prev == ch) {
          opt->o_abort = 1;
          opt->templates = realloc(opt->templates, strlen(opt->templates) + 2);
          opt->templates = strcat(opt->templates, ".");
        }
      }
      opt->templates = realloc(opt->templates, strlen(opt->templates) + 2);
      opt->templates[strlen(opt->templates)] = ch;
      opt->templates[strlen(opt->templates) + 1] = '\0';
    }
    fclose(fin);
    if (opt->templates[strlen(opt->templates) - 1] == '|') {
      opt->templates[strlen(opt->templates) - 1] = '\0';
    }
  } else {
    fprintf(stderr, "grep: %s: No such file or directory\n", optarg);
    if (opt->templates) {
      free(opt->templates);
    }
    exit(1);
  }
}

void output(FILE *fp, flag_opt *opt, int argc, char **argv, int j) {
  size_t nmatch = 1;
  size_t len = 0;
  regex_t reg;
  regmatch_t pmatch[1];
  int rc, lines_count = 0, n_count = 0, l_count = 0;
  char *str = NULL;
  int flag = REG_EXTENDED;
  if (opt->i) {
    flag |= REG_ICASE;
  }
  if ((rc = regcomp(&reg, opt->templates, flag)) == 0) {
    while (getline(&str, &len, fp) != -1) {
      n_count++;
      if ((rc = regexec(&reg, str, nmatch, pmatch, 0) == 0) && !opt->v) {
        if (opt->l || opt->c) {
          l_count = 1;
          if (opt->c) {
            lines_count++;
            continue;
          }
          continue;
        }
        if (!opt->h && argc - optind > 1) {
          printf("%s:", argv[j]);
        }
        if (opt->n) {
          printf("%d:", n_count);
        }
        if (opt->o && !opt->o_abort) {
          flag_o(str, reg, pmatch);
          continue;
        }
        printf("%s", str);
        if (str[strlen(str) - 1] != '\n') {
          printf("\n");
        }
      } else if (opt->v && rc == 0) {
        if (opt->l || opt->c) {
          l_count = 1;
          if (opt->c) {
            lines_count++;
            continue;
          }
          continue;
        }
        if (!opt->h && argc - optind > 1) {
          printf("%s:", argv[j]);
        }
        if (opt->n) {
          printf("%d:", n_count);
        }
        printf("%s", str);
        if (str[strlen(str) - 1] != '\n') {
          printf("\n");
        }
      }
    }

    if (opt->c) {
      if (!opt->h && argc - optind > 1) {
        printf("%s:", argv[j]);
      }
      if (opt->l && lines_count > 1) {
        lines_count = 1;
      }
      printf("%d\n", lines_count);
    }
    if (opt->l && l_count) {
      printf("%s\n", argv[j]); // test
    }
  }
  free(str);
  regfree(&reg);
}

void flag_o(char *str, regex_t reg, regmatch_t *pmatch) {
  while (1) {
    if (regexec(&reg, str, 1, pmatch, 0) != 0) {
      break;
    }
    for (int i = pmatch[0].rm_so; i < pmatch[0].rm_eo; ++i) {
      printf("%c", str[i]);
    }
    printf("\n");
    str += pmatch[0].rm_eo;
  }
}
