#include "pch.h"
#include "machine.h"

int main()
{
	machine m;

	m.add_job(0, 3, 0);
	m.add_job(18, 6, 0);
	m.add_job(12, 6, 0);
	m.add_job(8, 5, 0);
	m.add_job(8, 4, 0);
	m.add_job(17, 8, 0);
	m.add_job(16, 9, 0);

	m.add_child(1, std::vector<int>{2});
	m.add_child(3, std::vector<int>{4});
	m.add_child(4, std::vector<int>{5});
	m.add_child(6, std::vector<int>{7});

	m.print_jobs();

	m.init_tree(m.tardiness, false);

	m.init_tree(m.tardiness, true);

	//const int num_test = 10;
	//const int jobs_per_test = 4;

	//srand(time(nullptr));

	//for (int i = 0; i < num_test; ++i)
	//{
	//	for (int j = 0; j < jobs_per_test; ++j)
	//	{
	//		const int weight = rand() % 20 + 1;

	//		const int processing_time = rand() % 20 + 1;

	//		m.add_job(weight, processing_time, 0);
	//	}

	//	result r1, r2;

	//	m.print_jobs();
	//	r1 = m.init_tree(m.lateness);
	//	r2 = m.calculate_order();

	//	r1.print();
	//	r2.print();

	//	if (r1 == r2)
	//	{
	//		std::cout << "\nOK" << std::endl;
	//	}
	//	else
	//	{
	//		std::cout << "\nError" << std::endl;
	//	}

	//	m.erase_jobs();
	//}

	//m.init_tree(m.lateness);
	//m.init_tree(m.tardiness);
	//m.init_tree(m.total_weighted_tardiness);
}
