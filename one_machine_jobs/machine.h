#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <map>

struct job
{
	job() = default;

	int index;
	int weight;
	int processing_time;
	int due_date;
	double wpt;
};

class result
{
public:
	int optimum = INT_MAX;
	std::vector<int> res_order{};

	bool operator==(result &r1) const
	{
		return optimum == r1.optimum && res_order == r1.res_order;
	}

	void print()
	{
		std::cout << std::endl;
		std::cout << "optimum: " << optimum << std::endl;
		std::cout << "optimal order: ";
		for (auto it = res_order.begin(); it != res_order.end(); ++it)
		{
			std::cout << (*it) << " ";
		}
		std::cout << std::endl;
	}
};


class machine
{
public:
	machine() = default;
	~machine() = default;

	enum job_type
	{
		lateness = 0,
		tardiness = 1,
		total_weighted_tardiness = 2
	};


	std::map<int, std::vector<int>> adj_map;

	void fill() 
	{
		adj_map.insert(std::pair<int, std::vector<int> > (0, std::vector<int>{ 1, 3, 6 }));
		adj_map.insert(std::pair<int, std::vector<int> >(1, std::vector<int>{ 2 }));
		adj_map.insert(std::pair<int, std::vector<int> >(3, std::vector<int>{ 4 }));
		adj_map.insert(std::pair<int, std::vector<int> >(4, std::vector<int>{ 5 }));
		adj_map.insert(std::pair<int, std::vector<int> >(6, std::vector<int>{ 7 }));
	}

	void print_adj_map() 
	{
		for (auto it = adj_map.begin(); it != adj_map.end(); ++it) 
		{
			std::cout << (*it).first << " -> ";


			for (auto it_2 : (*it).second) 
			{
				std::cout << it_2 << " ";
			}

			std::cout << std::endl;
		}
		
	}

	void add_job(int w, int p, int d);
	void print_jobs();
	void erase_jobs();

	result calculate_order();

	result init_tree(const job_type &type);

private:
	int m_index = 1;
	int m_optimum = INT_MAX;
	std::vector<job> m_optimal_job{};
	std::vector<job> m_jobs{};
	static bool sort_by_wpt(job j1, job j2);


	void build_tree(std::vector<int> numbers, std::vector<job> permutations, const job_type &type);


};

