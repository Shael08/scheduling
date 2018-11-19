#include "pch.h"
#include "machine.h"

int main()
{
	machine m;
	m.add_job(6, 3, 3);
	m.add_job(11, 5, 5);
	m.add_job(9, 7, 11);
	m.add_job(5, 4, 12);
	m.print_jobs();

	m.init_tree(m.lateness);
	m.init_tree(m.tardiness);
}
