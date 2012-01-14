OS = $(shell echo %OS%)
ifeq ($(OS),%OS%)
	CC = gcc
else
	CC = cl
endif

all:
	$(CC) fakeShebang.c -o test_script
