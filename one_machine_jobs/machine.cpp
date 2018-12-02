#include "pch.h"
#include "machine.h"
#include <ostream>
#include <iostream>
#include <algorithm>

//void machine::fill_adj_map()
//{
//	adj_map.insert(std::pair<int, std::vector<int> >(0, std::vector<int>{ 1, 3, 6 }));
//	adj_map.insert(std::pair<int, std::vector<int> >(1, std::vector<int>{ 2 }));
//	adj_map.insert(std::pair<int, std::vector<int> >(3, std::vector<int>{ 4 }));
//	adj_map.insert(std::pair<int, std::vector<int> >(4, std::vector<int>{ 5 }));
//	adj_map.insert(std::pair<int, std::vector<int> >(6, std::vector<int>{ 7 }));
//}
//
//void machine::print_adj_map()
//{
//	for (auto it = adj_map.begin(); it != adj_map.end(); ++it)
//	{
//		std::cout << (*it).first << " -> ";
//
//
//		for (auto it_2 : (*it).second)
//		{
//			std::cout << it_2 << " ";
//		}
//
//		std::cout << std::endl;
//	}
//}

void machine::add_job(const double w, const double p, const double d)
{
	job j{};
	j.index = m_index;
	j.weight = w;
	j.processing_time = p;
	j.due_date = d;
	j.is_root = true;

	m_jobs.push_back(j);
	++m_index;
}

void machine::add_child(const int parent, const std::vector<int> child)
{
	if (parent < m_index) 
	{
		int proxy = m_index;
		if(std::any_of(child.begin(), child.end(), [proxy](const int i){return i >= proxy; }))
		{
			std::cout << "some child job does not exists" << std::endl;
		}
		else
		{
			for(auto it = child.begin(); it != child.end(); ++it)
			{
				m_jobs[parent - 1].child.push_back(*it);
				m_jobs[(*it) - 1].is_root = false;
			}

		}
	}
	else
	{
		std::cout << "parent job does not exists" << std::endl;
	}

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
		std::cout << "job: " << (*it).index << "\tweight: " << (*it).weight << "\tprocessing time: " << (*it).processing_time << "\tdue date: " << (*it).due_date << "\t is root: " << (*it).is_root << "\tchild: ";

		for(auto child = (*it).child.begin(); child != (*it).child.end(); ++child)
		{
			std::cout << (*child) << " ";
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
}

void machine::build_tree(std::set<int> numbers, std::vector<job> permutations, const job_type &type, const bool is_chain)
{
	if(is_chain)
	{
		if (!permutations.empty())
		{
			for (auto it = permutations.back().child.begin(); it != permutations.back().child.end(); ++it)
			{
				numbers.insert((*it) - 1);
			}
		}
	}

	if (m_optimum != INT_MAX || numbers.empty())
	{
		double sum = 0;
		double processing_time = 0;

		for (std::vector<job>::iterator it = permutations.begin(); it != permutations.end(); ++it)
		{
			double v;

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

		if (m_optimum < sum) return;

		if (numbers.empty())
		{
			m_optimal_job = permutations;
			m_optimum = sum;

			for (std::vector<job>::iterator it = permutations.begin(); it != permutations.end(); ++it)
			{
				std::cout << (*it).index << " ";
			}
			std::cout << "\nnew optimum found: " << sum << std::endl;

			return;
		}


	}

	for (std::set<int>::iterator it = numbers.begin(); it != numbers.end(); ++it)
	{
		const int value = (*it);
		permutations.push_back(m_jobs[*it]);
		it = numbers.erase(it);

		build_tree(numbers, permutations, type, is_chain);

		permutations.pop_back();
		it = numbers.insert(it, value);
	}

}

result machine::init_tree(const job_type &type, const bool is_chain)
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

	std::set<int> numbers;
	const std::vector<job> permutations;

	if (is_chain) 
	{
		for (auto it = m_jobs.begin(); it != m_jobs.end(); ++it)
		{
			if ((*it).is_root)
			{
				numbers.insert((*it).index - 1);
			}
		}

	}
	else
	{
		for (int i = 0; i < size; ++i)
		{
			numbers.insert(i);
		}
	}

	build_tree(numbers, permutations, type, is_chain);


	result r{};
	r.optimum = m_optimum;
	for (auto it = m_optimal_job.begin(); it != m_optimal_job.end(); ++it) {
		r.res_order.push_back((*it).index);
	}
	return r;
}


bool machine::sort_by_wpt(const job j1, const job j2)
{
	return j1.wpt > j2.wpt;
}

bool machine::get_max(const job j1, const job j2)
{
	return j1.wpt_in_chain.second > j2.wpt_in_chain.second;
}


result machine::calculate_optimal_order()
{

	for (std::vector<job>::iterator it = m_jobs.begin(); it != m_jobs.end(); ++it)
	{
		(*it).wpt = double((*it).weight) / double((*it).processing_time);
	}

	std::sort(m_jobs.begin(), m_jobs.end(), sort_by_wpt);

	double processing_time = 0, sum = 0;
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



double machine::calculate_wpt_in_chain(const double weight, const double processing_time, std::vector<int> child, const int parent)
{
	const double value = weight / processing_time;

	for(auto it = child.begin(); it != child.end(); ++it)
	{
		m_jobs[(*it) - 1].wpt_in_chain.first = m_jobs[parent].wpt_in_chain.first;
		m_jobs[(*it) - 1].wpt_in_chain.first.push_back((*it));
		m_jobs[(*it) - 1].wpt_in_chain.second = calculate_wpt_in_chain(m_jobs[(*it) - 1].weight + weight, m_jobs[(*it) - 1].processing_time + processing_time, m_jobs[(*it) - 1].child, (*it)-1);
	}

	return value;
}

result machine::calculate_optimal_order_in_chain()
{
	for (std::vector<job>::iterator it = m_jobs.begin(); it != m_jobs.end(); ++it)
	{
		if ((*it).is_root) 
		{	
			(*it).wpt_in_chain.first = std::vector<int>{ (*it).index };
			(*it).wpt_in_chain.second = calculate_wpt_in_chain((*it).weight, (*it).processing_time, (*it).child, (*it).index - 1);
		}
	}

	result r{};

	auto min_element= std::min_element(m_jobs.begin(), m_jobs.end(), get_max);

	while (!min_element->wpt_in_chain.first.empty())
	{
		const int last_element = min_element->wpt_in_chain.first.back() - 1;

		for (auto it = m_jobs[last_element].child.begin(); it != m_jobs[last_element].child.end(); ++it) 
		{
			m_jobs[(*it) - 1].wpt_in_chain.first.clear();
			m_jobs[(*it) - 1].wpt_in_chain.first.push_back((*it));
			m_jobs[(*it) - 1].wpt_in_chain.second = calculate_wpt_in_chain(m_jobs[(*it) - 1].weight, m_jobs[(*it) - 1].processing_time, m_jobs[(*it) - 1].child, (*it) - 1);
		}

		for(unsigned i = 0; i < min_element->wpt_in_chain.first.size(); ++i)
		{
			int index = min_element->wpt_in_chain.first[i] - 1;
			r.res_order.push_back(index);
			m_jobs[index].wpt_in_chain.first = std::vector<int>{ };
			m_jobs[index].wpt_in_chain.second = -1;
		}

		//std::cout << std::endl;
		//for (std::vector<job>::iterator it = m_jobs.begin(); it != m_jobs.end(); ++it)
		//{
		//	std::cout << "job: ";
		//	for (auto it_2 = (*it).wpt_in_chain.first.begin(); it_2 != (*it).wpt_in_chain.first.end(); ++it_2)
		//	{
		//		std::cout << (*it_2) << " ";
		//	}
		//	std::cout << "\twpt_in_chain: " << (*it).wpt_in_chain.second << std::endl;
		//}
		//std::cout << std::endl;

		min_element = std::min_element(m_jobs.begin(), m_jobs.end(), get_max);
	}

	int processing_time = 0, sum = 0;
	for (auto it = r.res_order.begin(); it != r.res_order.end(); ++it)
	{
		std::cout << m_jobs[(*it)].index << " ";
		processing_time += m_jobs[(*it)].processing_time;
		sum += m_jobs[(*it)].weight * processing_time;
	}
	r.optimum = sum;
	std::cout << "\noptimum: " << sum << std::endl;

	return r;
}