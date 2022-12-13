#include "../headers/sensores.h"

int main()
{
	srand(time(NULL));
    create_area();

	while (1)
	{
		printf("\e[1;1H\e[2J");
		print_area();
		put_fire();
		printf("\e[1;1H\e[2J");
		print_area();
		sleep(5);
	}

	destroy_threads();
	pthread_exit(NULL);
	return 0;
}
