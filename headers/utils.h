#ifndef UTILS_H
#include <unistd.h>
#include <assert.h>
#include <time.h>
#define UTILS_H
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


enum POSITION
{
	LEFT = 0,
	UP = 1,
	RIGHT = 2,
	BOTTOM = 3
};

#endif