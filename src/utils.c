#include "../headers/utils.h"

void randID(char *str)
{
#ifndef CHAR_LIMIT
#define MIN_CHAR_ASCII 97
#define MAX_CHAR_ASCII 122
#endif
    int i = 0;
    for (i = 0; i < MAX_WORD_LENGTH; ++i)
    {
        char rnd = (char)(MIN_CHAR_ASCII + (rand() % (MAX_CHAR_ASCII - MIN_CHAR_ASCII)));
        str[i] = rnd;
    }
    str[i + 1] = '\0';
}

CT get_time()
{
	time_t rawtime = time(NULL);
	struct tm *timeinfo;
    timeinfo = localtime(&rawtime);
    CT ct;
    ct.hour = timeinfo->tm_hour;
    ct.min = timeinfo->tm_min;
    ct.seconds = timeinfo->tm_sec;

    return ct;
}
