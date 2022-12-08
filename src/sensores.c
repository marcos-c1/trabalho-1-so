#include "../headers/sensores.h"
#include "../headers/utils.h"

/* OUTPUT STANDARD FILE */
FILE *out;

/* THREADS VARIABLES */
pthread_cond_t sensor, fire, bomb;
pthread_mutex_t lock;

/* GLOBAL VARIABLE */
int has_fire = FALSE;
int calling_central = FALSE;
int extinguish_fire = FALSE;

void *purge_fire(void *id)
{
	while (1)
	{
		pthread_mutex_lock(&lock);
		if (!extinguish_fire)
		{
			pthread_cond_wait(&bomb, &lock);
		}
		coord c;
		c.x = (*node_on_fire)->c.x;
		c.y = (*node_on_fire)->c.y;

		if (area[c.x + 1][c.y].id == 'X')
			area[c.x + 1][c.y].id = '-';
		else if (area[c.x - 1][c.y].id == 'X')
			area[c.x - 1][c.y].id = '-';
		else if (area[c.x][c.y + 1].id == 'X')
			area[c.x][c.y + 1].id = '-';
		else
			area[c.x][c.y - 1].id = '-';

		printf("Apagando o fogo...\n");
		sleep(5);
		extinguish_fire = 0;
		pthread_mutex_unlock(&lock);
	}
}

void *call_central(void *id)
{
	while (1)
	{
		pthread_mutex_lock(&lock);
		if (!calling_central)
		{
			pthread_cond_wait(&sensor, &lock);
		}

		if (((THREAD_NODE *)&node_on_fire)->isBorder)
		{
			pthread_cond_signal(&bomb);
		}

		printf("Iterando os vizinhos da thread sensor..\n");

		while (!(**node_on_fire).isBorder)
		{
			printf("proximo vizinho: (%d)(%d)\n", (*node_on_fire)->c.x, (*node_on_fire)->c.y);
			node_on_fire = (*node_on_fire)->vizinhos;
			node_on_fire = &node_on_fire[1];
			sleep(2);
		}
		extinguish_fire = 1;
		calling_central = 0;
		pthread_cond_signal(&bomb);
		pthread_mutex_unlock(&lock);
	}
}
void *check_fire(void *th)
{
	THREAD_NODE *t = (struct THREAD_NODE *)th;

	// printf("id: %c\t(%d, %d)\n", area[t->c.x][t->c.y].id, t->c.x, t->c.y);
	//  Precisa de uma instrução while(1)
	while (1)
	{
		pthread_mutex_lock(&lock);
		// printf("direita: %c\nesquerda: %c\ncima: %c\nbaixo: %c\nnoroeste: %c\nnordeste: %c\nsudoeste: %c\nsudeste: %c\n", area[t->c.x + 1][t->c.y].id, area[t->c.x - 1][t->c.y].id, area[t->c.x][t->c.y + 1].id, area[t->c.x][t->c.y - 1].id, area[t->c.x - 1][t->c.y + 1].id, area[t->c.x + 1][t->c.y + 1].id,area[t->c.x - 1][t->c.y - 1].id, area[t->c.x + 1][t->c.y + 1].id);
		if (area[t->c.x + 1][t->c.y].id == 'X' || area[t->c.x - 1][t->c.y].id == 'X' || area[t->c.x][t->c.y + 1].id == 'X' || area[t->c.x][t->c.y - 1].id == 'X')
		{
			sleep(1);
			printf("Incêndio na localidade (%d, %d), THREAD %ld\n", t->c.x, t->c.y, pthread_self());
			node_on_fire = &t;
			calling_central = 1;
			pthread_cond_signal(&sensor);
		} else {
			pthread_cond_wait(&fire, &lock);
		}
		pthread_mutex_unlock(&lock);
	}
}

void create_threads(int size)
{
	pthread_cond_init(&sensor, NULL);
	pthread_cond_init(&fire, NULL);
	pthread_cond_init(&bomb, NULL);

	pthread_t central, bombeiro;

	pthread_create(&central, NULL, call_central, NULL);
	pthread_create(&bombeiro, NULL, purge_fire, NULL);

	for (int i = 0; i < size; i++)
	{
		pthread_create(&threads[i].thread, NULL, check_fire, (void *)&threads[i]);
	}
	/*
	for(int i = 0; i < size; i++){
		pthread_join(threads[i].thread, NULL);
	}
	*/
}

void destroy_threads()
{
	pthread_mutex_destroy(&lock);
	pthread_cond_destroy(&sensor);
	pthread_cond_destroy(&fire);
	pthread_cond_destroy(&bomb);

	fclose(out);
}

coord get_thread_coord(int x, int y)
{
	coord c;

	if (x == 0)
	{
		if (y == 0)
		{
			c.x = x + 1;
			c.y = y + 1;
		}
		else if (y == WIDTH - 1)
		{
			c.x = x + 1;
			c.y = y - 1;
		}
		else if (area[x + 1][y - 1].id == 'T')
		{
			c.x = x + 1;
			c.y = y - 1;
		}
		else if (area[x + 1][y].id == 'T')
		{
			c.x = x + 1;
			c.y = y;
		}
		else
		{
			if (area[x + 1][y + 1].id == 'T')
			{
				c.x = x + 1;
				c.y = y + 1;
			}
		}
	}
	else if (x == HEIGHT - 1)
	{
		if (y == 0)
		{
			c.x = x - 1;
			c.y = y + 1;
		}
		else if (y == WIDTH - 1)
		{
			c.x = x - 1;
			c.y = y - 1;
		}
		else if (area[x - 1][y - 1].id == 'T')
		{
			c.x = x - 1;
			c.y = y - 1;
		}
		else if (area[x - 1][y].id == 'T')
		{
			c.x = x - 1;
			c.y = y;
		}
		else
		{
			if (area[x - 1][y + 1].id == 'T')
			{
				c.x = x + 1;
				c.y = y + 1;
			}
		}
	}
	else
	{
		if (area[x - 1][y - 1].id == 'T')
		{
			c.x = x - 1;
			c.y = y - 1;
		}
		else if (area[x + 1][y + 1].id == 'T')
		{
			c.x = x + 1;
			c.y = y + 1;
		}
		else if (area[x + 1][y - 1].id == 'T')
		{
			c.x = x + 1;
			c.y = y - 1;
		}
		else if (area[x - 1][y + 1].id == 'T')
		{
			c.x = x - 1;
			c.y = y + 1;
		}
		else
		{
			if (area[x][y + 1].id == 'T')
			{
				c.x = x;
				c.y = y + 1;
			}
			if (area[x][y - 1].id == 'T')
			{
				c.x = x;
				c.y = y - 1;
			}
			if (area[x + 1][y].id == 'T')
			{
				c.x = x + 1;
				c.y = y;
			}
			if (area[x - 1][y].id == 'T')
			{
				c.x = x - 1;
				c.y = y;
			}
		}
	}

	return c;
}
void put_fire()
{
	sleep(1);
	if (!has_fire)
	{
		coord c;
		c.x = rand() % WIDTH;
		c.y = rand() % HEIGHT;
		printf("Fogo em %d, %d\n", c.x, c.y);
		if (area[c.x][c.y].id == 'T')
		{
			area[c.x][c.y].t->isUp = FALSE;
			// LOG("Queimando uma thread");
		}
		else
		{
			area[c.x][c.y].id = 'X';
			coord tc = get_thread_coord(c.x, c.y);
			area[tc.x][tc.y].t->has_fire_around = TRUE;
			has_fire = 1;
			printf("Thread localizada na localidade (%d, %d)\n", tc.x, tc.y);
		}
	}
	pthread_cond_signal(&fire);
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
			threads[i].isBorder = TRUE;
		}
		if ((threads[i].c.x - 1 == 0) && (threads[i].isBorder == TRUE))
		{

			if (threads[i].c.y - 1 == 0)
			{
				threads[i].vizinhos = (THREAD_NODE **)malloc(sizeof(THREAD_NODE *) * 2);
				threads[i].vizinhos[0] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[1] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].qtd_vizinhos = 2;
				threads[i].vizinhos[0] = area[threads[i].c.x + 3][threads[i].c.y].t;
				threads[i].vizinhos[1] = area[threads[i].c.x][threads[i].c.y + 3].t;
			}
			else if (threads[i].c.y == WIDTH - 2)
			{
				threads[i].vizinhos = (THREAD_NODE **)malloc(sizeof(THREAD_NODE *) * 2);
				threads[i].qtd_vizinhos = 2;
				threads[i].vizinhos[0] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[1] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[0] = area[threads[i].c.x + 3][threads[i].c.y].t;
				threads[i].vizinhos[1] = area[threads[i].c.x][threads[i].c.y - 3].t;
			}
			else
			{
				threads[i].vizinhos = (THREAD_NODE **)malloc(sizeof(THREAD_NODE *) * 3);
				threads[i].qtd_vizinhos = 3;
				threads[i].vizinhos[0] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[1] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[2] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));

				threads[i].vizinhos[0] = area[threads[i].c.x + 3][threads[i].c.y].t;
				threads[i].vizinhos[1] = area[threads[i].c.x][threads[i].c.y + 3].t;
				threads[i].vizinhos[2] = area[threads[i].c.x][threads[i].c.y - 3].t;
			}
		}
		else if ((threads[i].c.x == HEIGHT - 2) && (threads[i].isBorder == TRUE))
		{
			if (threads[i].c.y - 1 == 0)
			{
				threads[i].vizinhos = (THREAD_NODE **)malloc(sizeof(THREAD_NODE *) * 2);
				threads[i].qtd_vizinhos = 2;
				threads[i].vizinhos[0] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[1] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));

				threads[i].vizinhos[0] = area[threads[i].c.x - 3][threads[i].c.y].t;
				threads[i].vizinhos[1] = area[threads[i].c.x][threads[i].c.y + 3].t;
			}
			else if (threads[i].c.y == WIDTH - 2)
			{
				threads[i].vizinhos = (THREAD_NODE **)malloc(sizeof(THREAD_NODE *) * 2);
				threads[i].qtd_vizinhos = 2;
				threads[i].vizinhos[0] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[1] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));

				threads[i].vizinhos[0] = area[threads[i].c.x - 3][threads[i].c.y].t;
				threads[i].vizinhos[1] = area[threads[i].c.x][threads[i].c.y - 3].t;
			}
			else
			{
				threads[i].vizinhos = (THREAD_NODE **)malloc(sizeof(THREAD_NODE *) * 3);
				threads[i].qtd_vizinhos = 3;
				threads[i].vizinhos[0] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[1] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[2] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));

				threads[i].vizinhos[0] = area[threads[i].c.x - 3][threads[i].c.y].t;

				threads[i].vizinhos[1] = area[threads[i].c.x][threads[i].c.y + 3].t;
				threads[i].vizinhos[2] = area[threads[i].c.x][threads[i].c.y - 3].t;
			}
		}
		else
		{
			if (threads[i].c.y - 1 == 0 && threads[i].isBorder == TRUE)
			{
				threads[i].vizinhos = (THREAD_NODE **)malloc(sizeof(THREAD_NODE *) * 3);
				threads[i].qtd_vizinhos = 3;
				threads[i].vizinhos[0] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[1] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[2] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));

				threads[i].vizinhos[0] = area[threads[i].c.x - 3][threads[i].c.y].t;
				threads[i].vizinhos[1] = area[threads[i].c.x][threads[i].c.y + 3].t;
				threads[i].vizinhos[2] = area[threads[i].c.x + 3][threads[i].c.y].t;
			}
			if (threads[i].c.y == WIDTH - 2 && threads[i].isBorder == TRUE)
			{
				threads[i].vizinhos = (THREAD_NODE **)malloc(sizeof(THREAD_NODE *) * 3);
				threads[i].qtd_vizinhos = 3;
				threads[i].vizinhos[0] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[1] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
				threads[i].vizinhos[2] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));

				threads[i].vizinhos[0] = area[threads[i].c.x - 3][threads[i].c.y].t;
				threads[i].vizinhos[1] = area[threads[i].c.x][threads[i].c.y - 3].t;
				threads[i].vizinhos[2] = area[threads[i].c.x + 3][threads[i].c.y].t;
			}
		}
	}

	int k = 3;

	for (int i = 0; i < size; i++)
	{
		int x = threads[i].c.x;
		int y = threads[i].c.y;
		int isBorder = threads[i].isBorder;

		if (!isBorder)
		{
			threads[i].vizinhos = (THREAD_NODE **)malloc(sizeof(THREAD_NODE *) * 4);

			threads[i].vizinhos[0] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
			threads[i].vizinhos[1] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
			threads[i].vizinhos[2] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
			threads[i].vizinhos[3] = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));

			threads[i].qtd_vizinhos = 4;

			if ((x != 1 || x != HEIGHT - 2))
			{
				threads[i].vizinhos[0] = area[x - k][y].t;
				threads[i].vizinhos[1] = area[x + k][y].t;
			}

			if ((y != 1 || y != WIDTH - 2))
			{
				threads[i].vizinhos[2] = area[x][y + k].t;
				threads[i].vizinhos[3] = area[x][y - k].t;
			}
		}
	}
}

void print_neigh()
{
	for (int i = 0; i < MAX_THREADS; i++)
	{
		printf("Qtd de vizinhos da thread %d: %d\n", i, threads[i].qtd_vizinhos);
		printf("Localidade da thread %d: (%d, %d)\n", i, threads[i].c.x, threads[i].c.y);
		for (int j = 0; j < threads[i].qtd_vizinhos; j++)
		{
			printf("\t(%d, %d)\n", threads[i].vizinhos[j]->c.x, threads[i].vizinhos[j]->c.y);
		}
		printf("\n");
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
				threads[it_t].has_fire_around = FALSE;
				area[i][j].id = 'T';
				area[i][j].t = (THREAD_NODE *)malloc(sizeof(THREAD_NODE));
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
	int i, j;

	for (i = 0; i < WIDTH; i++)
	{
		for (j = 0; j < HEIGHT; j++)
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
