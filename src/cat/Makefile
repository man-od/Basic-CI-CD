all: s21_cat
s21_cat : 
	gcc -Wall -Werror -Wextra -D_GNU_SOURCE s21_cat.c -o s21_cat

clean :
	rm -rf s21_cat
	rm -rf *.log

rebuild:
	$(MAKE) clean
	$(MAKE) all
tests :
	chmod +x test_func_cat.sh
	./test_func_cat.sh
