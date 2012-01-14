ifdef OS
	CC = cl
	OUT = /Fetest_script
else
	CC = gcc
	OUT = -o test_script
endif

all:
	$(CC) fakeShebang.c $(OUT)
