all: s21_grep
s21_grep :
	gcc -Wall -Werror -Wextra -D_GNU_SOURCE s21_grep.c -o s21_grep

clean: 
	rm -rf s21_grep
	rm -rf *.log

rebuild:
	$(MAKE) clean
	$(MAKE) all
tests:
	chmod 777 test_func_grep.sh
	bash test_func_grep.sh
