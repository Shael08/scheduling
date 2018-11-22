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

void machine::print_jobs()
{
	for (std::vector<job>::iterator it = jobs_.begin(); it != jobs_.end(); ++it)
	{
		std::cout << "job: " << (*it).index << "\tweight: " << (*it).weight << "\tprocessing time: " << (*it).processing_time << "\tdue date: " << (*it).due_date << std::endl;
	}
	std::cout << std::endl;
}

void machine::build_tree(std::vector<int> numbers, std::vector<job> permutations, const job_type &type)
{
	if (optimum_ != INT_MAX || numbers.empty())
	{
		int sum = 0;
		int processing_time = 0;

		for (std::vector<job>::iterator it = permutations.begin(); it != permutations.end(); ++it)
		{
			int v = 0;

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

		if (optimum_ <= sum) return;

		if (numbers.empty() && optimum_ > sum)
		{
			optimal_job_ = permutations;
			optimum_ = sum;

			for (std::vector<job>::iterator it = permutations.begin(); it != permutations.end(); ++it)
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

void machine::init_tree(const job_type &type)
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
		return;
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
}