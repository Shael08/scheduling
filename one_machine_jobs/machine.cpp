#include "pch.h"
#include "machine.h"
#include <algorithm>
#include <ostream>
#include <iostream>

void machine::add_job(const int w, const int p, const int d)
{
	job j;
	j.index = index_;
	j.weight = w;
	j.processing_time = p;
	j.due_date = d;
	jobs_.push_back(j);
	++index_;
}

void machine::erase_jobs()
{
	jobs_.clear();
}

void machine::print_jobs()
{
	for (std::vector<job>::iterator it = jobs_.begin(); it != jobs_.end(); ++it)
	{
		std::cout << "job: " << (*it).index << "\tweight: " << (*it).weight << "\tprocessing time: " << (*it).processing_time << "\tdue date: " << (*it).due_date << std::endl;
	}
	std::cout << std::endl;
}

bool machine::sort_by_wpt(job j1, job j2)
{
	return j1.wpt > j2.wpt;
}

void machine::build_tree(std::vector<int> numbers, std::vector<job> permutations, const job_type &type)
{
	if (optimum_ != INT_MAX || numbers.empty())
	{
		int sum = 0;
		int processing_time = 0;

		//for (std::vector<job>::iterator it = permutations.begin(); it != permutations.end(); ++it)
		//{
		//	std::cout << (*it).index << " ";
		//}
		//std::cout << std::endl;
		for (std::vector<job>::iterator it = permutations.begin(); it != permutations.end(); ++it)
		{

			if (type == lateness) {
				processing_time += (*it).processing_time;
				sum += (*it).weight * processing_time;
			}
			else
			{
				processing_time += (*it).processing_time;
				const int v = processing_time - (*it).due_date;
				sum += v < 0 ? 0 : v;
			}
		}

		if (optimum_ <= sum)
		{
			if (!numbers.empty()) 
			{
				std::cout << "pruning" << std::endl;
			}
			return;

		}

		if(numbers.empty() && optimum_ > sum)
		{
			optimal_job_ = permutations;
			optimum_ = sum;

			for (std::vector<job>::iterator it = optimal_job_.begin(); it != optimal_job_.end(); ++it) 
			{
				std::cout << (*it).index << " ";
			}

			std::cout << "\nnew optimum found: " << sum << std::endl;

			return;
		}
	}

	for (std::vector<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
	{
		const int value = (*it);
		permutations.push_back(jobs_[*it]);
		it = numbers.erase(it);

		build_tree(numbers, permutations, type);

		permutations.pop_back();
		it = numbers.insert(it, value);
	}

}

result machine::init_tree(const job_type &type)
{
	if(type == lateness)
	{
		std::cout << "finding shortest processing time: " << std::endl;
	}
	else
	{
		std::cout << "finding optimum by due date: " << std::endl;
	}

	optimum_ = INT_MAX;

	const int size = jobs_.size();

	std::vector<int> numbers;
	const std::vector<job> permutations;

	for (int i = 0; i < size; ++i)
	{
		numbers.push_back(i);
	}


	build_tree(numbers, permutations, type);

	result r;
	r.optimum = optimum_;
	//std::copy(optimal_job_.begin(), optimal_job_.end(), r.res_order);
	return r;
}

result machine::calculate_order()
{

	for (std::vector<job>::iterator  it = jobs_.begin(); it != jobs_.end(); ++it) 
	{
		(*it).wpt = double((*it).weight) / double((*it).processing_time);
	}

	std::sort(jobs_.begin(), jobs_.end(), sort_by_wpt);

	int processing_time = 0, sum = 0;
	result r;

	for (std::vector<job>::iterator it = jobs_.begin(); it != jobs_.end(); ++it)
	{
		r.res_order.push_back((*it).index);
		std::cout << (*it).index << " " << (*it).wpt << std::endl;
		processing_time += (*it).processing_time;
		sum += (*it).weight * processing_time;
	}
	r.optimum = sum;
	std::cout << "\noptimum: " << sum << std::endl;

	return r;
}
