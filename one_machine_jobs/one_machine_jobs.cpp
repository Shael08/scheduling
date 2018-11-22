#include "pch.h"
#include "machine.h"

int main()
{
	machine m;
	m.add_job(4, 12, 16);
	m.add_job(5, 8, 26);
	m.add_job(3, 15, 25);
	m.add_job(5, 9, 27);
	m.print_jobs();

	//m.init_tree(m.lateness);
	//m.init_tree(m.tardiness);
	m.init_tree(m.total_weighted_tardiness);
}
