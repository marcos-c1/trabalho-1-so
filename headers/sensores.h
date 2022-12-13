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

/* STRUCTURES */
typedef struct
{
	int x;
	int y;
} coord;

typedef struct THREAD_NODE
{
	coord c;
	pthread_t thread;
	struct THREAD_NODE **vizinhos;
	int isUp;
	char *hash;
	int isBorder;
	int qtd_vizinhos;
} THREAD_NODE;

typedef struct
{
	THREAD_NODE *t;
	char id;
} THREAD_NODES;

/* STATIC VARIABLES */
static THREAD_NODES area[WIDTH][HEIGHT];
static THREAD_NODE threads[MAX_THREADS];
static THREAD_NODE **node_on_fire;


/* FUNCTIONS */
void create_area();
void print_area();
void create_neigh(int size);
void print_border(int size);
void put_fire();
void create_threads(int size);
void *check_fire(void *thread);
void destroy_threads();
void print_neigh();
coord check_neigh_fire(coord c);

#endif
