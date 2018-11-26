#include "pch.h"
#include "machine.h"
#include <ostream>
#include <iostream>
#include <algorithm>

void machine::add_job(const int w, const int p, const int d)
{
	job j{};
	j.index = m_index;
	j.weight = w;
	j.processing_time = p;
	j.due_date = d;

	m_jobs.push_back(j);
	++m_index;
}

void machine::erase_jobs()
{
	m_jobs.clear();
	m_index = 0;
}

void machine::print_jobs()
{
	std::cout << std::endl;
	for (std::vector<job>::iterator it = m_jobs.begin(); it != m_jobs.end(); ++it)
	{
		std::cout << "job: " << (*it).index << "\tweight: " << (*it).weight << "\tprocessing time: " << (*it).processing_time << "\tdue date: " << (*it).due_date << std::endl;
	}
	std::cout << std::endl;
}

void machine::build_tree(std::vector<int> numbers, std::vector<job> permutations, const job_type &type)
{
	if (m_optimum != INT_MAX || numbers.empty())
	{
		int sum = 0;
		int processing_time = 0;

		for (std::vector<job>::iterator it = permutations.begin(); it != permutations.end(); ++it)
		{
			int v;

			switch(type)
			{
			case lateness:
				processing_time += (*it).processing_time;
				sum += (*it).weight * processing_time;
				break;

			case tardiness:
				processing_time += (*it).processing_time;
				v = processing_time - (*it).due_date;
				sum += v < 0 ? 0 : v;
				break;

			case total_weighted_tardiness:
				processing_time += (*it).processing_time;
				v = processing_time - (*it).due_date;
				sum += v < 0 ? 0 : v * (*it).weight;
				break;
			default:
				break;
			}
		}

		if (m_optimum <= sum) return;

		if (numbers.empty() && m_optimum > sum)
		{
			m_optimal_job = permutations;
			m_optimum = sum;

			//for (std::vector<job>::iterator it = permutations.begin(); it != permutations.end(); ++it)
			//{
			//	std::cout << (*it).index << " ";
			//}
			//std::cout << "\nnew optimum found: " << sum << std::endl;

			return;
		}


	}

	for (std::vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
	{
		const int value = (*it);
		permutations.push_back(m_jobs[*it]);
		it = numbers.erase(it);

		build_tree(numbers, permutations, type);

		permutations.pop_back();
		it = numbers.insert(it, value);
	}

}

result machine::init_tree(const job_type &type)
{
	switch (type)
	{
	case  lateness:
		std::cout << "finding shortest processing time: " << std::endl;
		break;

	case  tardiness:
		std::cout << "finding optimum by due date: " << std::endl;
		break;

	case total_weighted_tardiness:
		std::cout << "finding optimum by total weighted tardiness: " << std::endl;
		break;

	default:
		return result{};
	}

	m_optimum = INT_MAX;

	const int size = m_jobs.size();

	std::vector<int> numbers;
	const std::vector<job> permutations;

	for (int i = 0; i < size; ++i)
	{
		numbers.push_back(i);
	}


	build_tree(numbers, permutations, type);


	result r{};
	r.optimum = m_optimum;
	for (auto it = m_optimal_job.begin(); it != m_optimal_job.end(); ++it) {
		r.res_order.push_back((*it).index);
	}
	return r;
}


bool machine::sort_by_wpt(job j1, job j2)
{
	return j1.wpt > j2.wpt;
}

result machine::calculate_order()
{

	for (std::vector<job>::iterator it = m_jobs.begin(); it != m_jobs.end(); ++it)
	{
		(*it).wpt = double((*it).weight) / double((*it).processing_time);
	}

	std::sort(m_jobs.begin(), m_jobs.end(), sort_by_wpt);

	int processing_time = 0, sum = 0;
	result r{};

	for (std::vector<job>::iterator it = m_jobs.begin(); it != m_jobs.end(); ++it)
	{
		r.res_order.push_back((*it).index);
		//std::cout << (*it).index << " " << (*it).wpt << std::endl;
		processing_time += (*it).processing_time;
		sum += (*it).weight * processing_time;
	}
	r.optimum = sum;
	//std::cout << "\noptimum: " << sum << std::endl;

	return r;
}