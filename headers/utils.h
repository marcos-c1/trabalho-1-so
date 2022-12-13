#ifndef UTILS_H
#include <unistd.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define UTILS_H
#define MAX_WORD_LENGTH 15
#define MAX_BUFFER_DIR 100
#define TRUE 1
#define FALSE 0
#define GREEN(string) "\x001b[42;1m" \
					  "\x001b[30m" string "\x1b[0m"
#define RED(string) "\x001b[41;1m" \
					"\x001b[30m" string "\x1b[0m"
#define LOG(msg)                    \
	{                               \
		fprintf(stdout, "%s", msg); \
	}

typedef struct CT {
	int hour;
	int min;
	int seconds;
}CT;

enum POSITION
{
	LEFT = 0,
	UP = 1,
	RIGHT = 2,
	BOTTOM = 3
};

CT get_time();
void randID(char *str);
unsigned int hash(size_t length, const char *str);

#endif