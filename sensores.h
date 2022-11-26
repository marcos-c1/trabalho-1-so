#ifndef SENSORES_H
#define SENSORES_H
#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include "utils.h"
#define WIDTH 30
#define HEIGHT 30
#define MAX_THREADS 100

typedef struct
{
	int x;
	int y;
} coord;

typedef struct
{
	coord c;
	pthread_t thread;
	struct THREAD_NODE **vizinhos;
	int isUp;
	int isBorder;
} THREAD_NODE;

typedef struct
{
	THREAD_NODE *t;
	char id;
} THREAD_NODES;

static THREAD_NODES area[WIDTH][HEIGHT];
static THREAD_NODE threads[MAX_THREADS];

/* FUNCTIONS */
void create_area();
void print_area();
void get_time();
void create_neigh(int size);
void print_border(int size);
void put_fire();
void create_threads(int size);
void *check_neighbour(void *thread);

#endif