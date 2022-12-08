#include "../headers/sensores.h"

int main()
{
	srand(time(NULL));
    create_area();

	while (1)
	{
		put_fire();
		//print_area();
		sleep(3);
		//system("clear");
	}

	destroy_threads();
	pthread_exit(NULL);
	return 0;
}
