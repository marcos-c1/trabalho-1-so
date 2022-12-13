#include "../headers/sensores.h"

int main()
{
	srand(time(NULL));
    create_area();

	while (1)
	{
		system("clear");
		print_area();
		put_fire();
		fflush(stdout);
		system("clear");
		print_area();
		sleep(5);
	}

	destroy_threads();
	pthread_exit(NULL);
	return 0;
}
