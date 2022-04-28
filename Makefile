all: tm
tm: tm.c tm.h
	gcc -Wall -Wextra -std=c11 tm.c -o tm