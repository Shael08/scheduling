#pragma once
#include <vector>
#include <iostream>
#include <list>
#include <map>
#include <set>

struct job
{
	job() = default;

	int index;
	double weight;
	double processing_time;
	double due_date;
	double wpt;
	std::vector<int> child;
	bool is_root;
	std::pair<std::vector<int> ,double> wpt_in_chain;
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


	//std::map<int, std::vector<int>> adj_map{};
	//void fill_adj_map();
	//void print_adj_map();

	
	void add_job(double w, double p, double d);
	void add_child(int parent, std::vector<int> child);
	void print_jobs();
	void erase_jobs();

	result calculate_optimal_order();
	result init_tree(const job_type &type, bool is_chain = false);
	result calculate_optimal_order_in_chain();

private:
	int m_index = 1;
	double m_optimum = INT_MAX;
	std::vector<job> m_optimal_job{};
	std::vector<job> m_jobs{};
	static bool sort_by_wpt(job j1, job j2);
	static bool get_max(job j1, job j2);
	double calculate_wpt_in_chain(double weight, double processing_time, std::vector<int> child, int parent);

	void build_tree(std::set<int> numbers, std::vector<job> permutations, const job_type &type, bool is_chain);
};

