#include "pch.h"
#include "machine.h"
#include <time.h>  
#include <iostream>

int main()
{
	machine m;

	int num_test = 10;
	int jobs_per_test = 4;

	srand(time(NULL));

	for (int i = 0; i < num_test; ++i) 
	{
		for (int j = 0; j < jobs_per_test; ++j) 
		{
			int weight = rand() % 20 + 1;

			int processing_time = rand() % 20 + 1;

			m.add_job(weight, processing_time, 0);
		}

		result r1, r2;

		m.print_jobs();
		r1 = m.init_tree(m.lateness);
		r2 = m.calculate_order();

		if (r1 == r2) 
		{
			std::cout << "OK" << std::endl;
		}
		else 
		{
			std::cout << "Error" << std::endl;
		}

		m.erase_jobs();
	}

	//m.add_job(6, 3, 3);
	//m.add_job(11, 5, 5);
	//m.add_job(9, 7, 11);
	//m.add_job(5, 4, 12);


//	m.init_tree(m.tardiness);
}
