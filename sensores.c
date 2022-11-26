#include "sensores.h"
#include "utils.h"

void *check_neighbour(void *thread)
{
	THREAD_NODE *t = thread;
	int k = 1;
	sleep(1);

	if (area[t->c.x + 1][t->c.y].id == 'X' || area[t->c.x - 1][t->c.y].id == 'X' || area[t->c.x][t->c.y + 1].id == 'X' || area[t->c.x][t->c.y - 1].id == 'X')
		{
			printf("Incêndio na localidade (%d, %d), THREAD %ld\n", t->c.x, t->c.y, pthread_self());
		}
}

void create_threads(int size)
{
	for (int i = 0; i < size; i++)
	{
		if (!threads[i].isBorder)
			pthread_create(&threads[i].thread, NULL, check_neighbour, (void *)&threads[i]);
	}
	/*
	for(int i = 0; i < size; i++){
		if(!threads[i].isBorder)
			pthread_join(threads[i].thread, NULL);
	}
	*/
}

void put_fire()
{
	sleep(5);
	coord c;
	c.x = rand() % WIDTH;
	c.y = rand() % HEIGHT;
	if (area[c.x][c.y].id == 'T')
	{
		// LOG("Queimando uma thread");
	}
	area[c.x][c.y].id = 'X';
}

void print_border(int size)
{
	for (int i = 0; i < size; i++)
	{
		if (threads[i].isBorder)
			printf("THREAD DA BORDA (%d, %d)\n", threads[i].c.x, threads[i].c.y);
	}
}

void create_neigh(int size)
{
	assert(size >= 2);
	for (int i = 0; i < size; i++)
	{
		if (((threads[i].c.x - 1 == 0) || (threads[i].c.x + 1 == HEIGHT - 1)) || ((threads[i].c.y + 1 == WIDTH - 1) || (threads[i].c.y - 1 == 0)))
		{
			int x = threads[i].c.x;
			int y = threads[i].c.y;
			threads[i].isBorder = TRUE;
		}
	}

	int k = 3;

	for (int i = 0; i < size; i++)
	{
		int x = threads[i].c.x;
		int y = threads[i].c.y;
		int isBorder = threads[i].isBorder;

		threads[i].vizinhos = malloc(sizeof(THREAD_NODE) * 4);

		if ((x != 1 || x != HEIGHT - 2) && !isBorder)
		{
			threads[i].vizinhos[UP] = (struct THREAD_NODE *)area[x - k][y].t;
			threads[i].vizinhos[BOTTOM] = (struct THREAD_NODE *)area[x + k][y].t;
		}

		if ((y != 1 || y != WIDTH - 2) && !isBorder)
		{
			threads[i].vizinhos[RIGHT] = (struct THREAD_NODE *)area[x][y + k].t;
			threads[i].vizinhos[LEFT] = (struct THREAD_NODE *)area[x][y - k].t;
		}
	}
}

void create_area()
{
	int thread_node = FALSE;
	int i, k, j, t;
	int it_t = 0;
	for (i = 0, k = 1; i < WIDTH; i++)
	{
		if (i == k)
		{
			thread_node = TRUE;
			k += 3;
		}
		else
		{
			thread_node = FALSE;
		}
		for (j = 0, t = 1; j < HEIGHT; j++)
		{
			if (j == t && thread_node)
			{
				threads[it_t].c.x = i;
				threads[it_t].c.y = j;
				threads[it_t].isUp = TRUE;
				area[i][j].id = 'T';
				area[i][j].t = &threads[it_t];
				it_t++;
				t += 3;
			}
			else
			{
				area[i][j].id = '-';
				area[i][j].t = NULL;
			}
		}
	}
	create_neigh(it_t);
	create_threads(it_t);
}

void print_area()
{
	int thread_node = FALSE;
	int i, k, j, t;

	for (i = 0, k = 1; i < WIDTH; i++)
	{
		for (j = 0, t = 1; j < HEIGHT; j++)
		{
			if (area[i][j].id == '-' || area[i][j].id == 'T')
				printf(GREEN(" %c "), area[i][j].id);
			else if (area[i][j].id == 'X')
				printf(RED(" %c "), area[i][j].id);
		}
		printf("\n");
	}
}

void get_time()
{
	time_t rawtime = time(NULL);
	struct tm *timeinfo;
	timeinfo = localtime(&rawtime);
	printf("[%d:%d:%d]\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);
}