#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <time.h>
#include <string.h>
#define TRUE 1
#define FALSE 0
#define WIDTH 30
#define HEIGHT 30
#define MAX_THREADS 100
#define GREEN(string) "\x001b[42;1m" "\x001b[30m" string "\x1b[0m"
#define RED(string) "\x001b[41;1m" "\x001b[30m" string "\x1b[0m"

char area[WIDTH][HEIGHT];

typedef struct {
	int x;
	int y;
}coord;

typedef struct {
	coord c;
	pthread_t thread;
	struct THREAD_NODE *vizinhos;
	int isUp;
}THREAD_NODE;

THREAD_NODE threads[MAX_THREADS];

coord put_fire()
{
	coord c;
	c.x = rand() % WIDTH;
	c.y = rand() % HEIGHT;
	return c;	
}

void create_area()
{
		int thread_node = FALSE;
    	int i, k, j, t;
		int it_t = 0;
    	for(i = 0, k = 1; i < WIDTH; i++)
    	{
    		if(i == k){
    			thread_node = TRUE;
    			k += 3;
    		} else {
    			thread_node = FALSE;
    		}
    		for(j = 0, t = 1; j < HEIGHT; j++){
    			if(j == t && thread_node){ 
					threads[it_t].c.x = i;
					threads[it_t].c.y = j;
					threads[it_t].isUp = TRUE;
					area[i][j] = 'T'; 
					it_t++;
    				t += 3;
    			}
    			else
					area[i][j] = '-';
    		}
    	}
}

void print_area()
{
	int thread_node = FALSE;
	int i, k, j, t;
	
	for(i = 0, k = 1; i < WIDTH; i++)
	{
		for(j = 0, t = 1; j < HEIGHT; j++){
			if(area[i][j] == '-' || area[i][j] == 'T')
				printf(GREEN(" %c "), area[i][j]); 
			else if(area[i][j] == 'X')
				printf(RED(" %c "), area[i][j]);
		}
		printf("\n");
	}
}

void print_threads()
{
	for(int i = 0; i < WIDTH; i++){
		for(int j = 0; j < HEIGHT; j++){
			if(area[i][j] == 'T'){
				printf("THREAD: %d, %d\n", i, j);
			}
		}
	}
}
int check_thread_pos(int x, int y)
{
	if(area[x][y] == 'T'){}
}

void get_time(){
	time_t rawtime = time(NULL);
	struct tm *timeinfo;
	timeinfo = localtime(&rawtime);	
	printf("[%d:%d:%d]\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec); 
}

int main()
{
	create_area();
	print_threads();	
	get_time();
/*	while(1){
		coord c = put_fire();
		area[c.x][c.y] = 'X';
		print_area();
		fflush(stdout);
		sleep(1);
		printf("\r");
		sleep(1);
		
	}*/
	return 0;
}

