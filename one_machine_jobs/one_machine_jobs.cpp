#include "pch.h"
#include "machine.h"
#include <ostream>
#include <iostream>
#include <time.h>

int main()
{
	machine m;

	const int num_test = 10;
	const int jobs_per_test = 4;

	srand(time(nullptr));

	for (int i = 0; i < num_test; ++i)
	{
		for (int j = 0; j < jobs_per_test; ++j)
		{
			const int weight = rand() % 20 + 1;

			const int processing_time = rand() % 20 + 1;

			m.add_job(weight, processing_time, 0);
		}

		result r1, r2;

		m.print_jobs();
		r1 = m.init_tree(m.lateness);
		r2 = m.calculate_order();

		r1.print();
		r2.print();

		if (r1 == r2)
		{
			std::cout << "\nOK" << std::endl;
		}
		else
		{
			std::cout << "\nError" << std::endl;
		}

		m.erase_jobs();
	}

	//m.init_tree(m.lateness);
	//m.init_tree(m.tardiness);
	//m.init_tree(m.total_weighted_tardiness);
}
