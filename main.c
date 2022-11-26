#include "sensores.h"

int main()
{
	srand(time(NULL));
    create_area();

	while (1)
	{
		put_fire();
		print_area();
		sleep(5);
		system("clear");
	}
	return 0;
}